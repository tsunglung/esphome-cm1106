#include "cm1106x.h"
#include "esphome/core/log.h"

namespace esphome {
namespace cm1106x {

static const char *const TAG = "cm1106x";

uint8_t CM1106_CMD_GET_CO2[4] = {0x11, 0x01, 0x01, 0xED}; // head, len, cmd, [data], crc
uint8_t CM1106_CMD_SET_CO2_CALIB[6] = {0x11, 0x03, 0x03, 0x00, 0x00, 0x00};
uint8_t CM1106_CMD_SET_CO2_CALIB_RESPONSE[4] = {0x16, 0x01, 0x03, 0xE6};

static const uint8_t OUTPUT_REGISTER = 12;
static const uint8_t INPUT_REG = 12;

float CM1106xComponent::get_setup_priority() const { return setup_priority::DATA; }

void CM1106xComponent::setup() { ESP_LOGCONFIG(TAG, "Setting up CM1106x..."); }

void CM1106xComponent::update() { 
    int16_t ppm = this->get_co2_ppm();
    if(ppm > -1) {
        this->co2_sensor_->publish_state(ppm);
    }
}

void CM1106xComponent::loop() {

}

void CM1106xComponent::set_co2_calibration(uint16_t ppm) {
    uint8_t cmd[6];

    memcpy(cmd, CM1106_CMD_SET_CO2_CALIB, sizeof(cmd));
    cmd[3] = ppm >> 8;
    cmd[4] = ppm & 0xFF;
    uint8_t response[4] = {0};
    bool success = this->send_command(cmd, sizeof(cmd), response, sizeof(response));

    if(!success) {
        ESP_LOGW(TAG, "Reading data from CM1106 failed!");
        return;
    }

    // check if correct response received
    if(memcmp(response, CM1106_CMD_SET_CO2_CALIB_RESPONSE, sizeof(response)) != 0) {
        ESP_LOGW(TAG, "Got wrong UART response: %02X %02X %02X %02X", response[0], response[1], response[2], response[3]);
        return;
    }

    ESP_LOGD(TAG, "CM1106 Successfully calibrated sensor to %uppm", ppm);

}

int16_t CM1106xComponent::get_co2_ppm() {
    uint8_t response[8] = {0}; // response: 0x16, 0x05, 0x01, DF1, DF2, DF3, DF4, CRC. PPM: DF1*256+DF2
    bool success = this->send_command(CM1106_CMD_GET_CO2, sizeof(CM1106_CMD_GET_CO2), response, sizeof(response));
    
    if(!success) {
        ESP_LOGW(TAG, "Reading data from CM1106 failed!");
        return -1;
    }

    if(!(response[0] == 0x16 && response[1] == 0x05 && response[2] == 0x01)) {
        ESP_LOGW(TAG, "Got wrong UART response: %02X %02X %02X %02X...", response[0], response[1], response[2], response[3]);
        return -1;
    }

    uint8_t checksum = this->check_crc_(response, sizeof(response));
    if(response[7] != checksum) {
        ESP_LOGW(TAG, "Got wrong UART checksum: 0x%02X - Calculated: 0x%02X", response[7], checksum);
        return -1;
    }

    int16_t ppm = response[3] << 8 | response[4];

    ESP_LOGD(TAG, "CM1106 Received CO₂=%uppm DF3=%02X DF4=%02X", ppm, response[5], response[6]);

    return ppm;
}

// Checksum: 256-(HEAD+LEN+CMD+DATA)%256
uint8_t CM1106xComponent::check_crc_(uint8_t* response, size_t len) {
    uint8_t crc = 0;
    // last byte of response is checksum, don't calculate it
    for(int i = 0; i < len - 1; i++) {
        crc -= response[i];
    }
    return crc;
}

bool CM1106xComponent::send_command(uint8_t *command, size_t commandLen, uint8_t *response, size_t responseLen) {
    // Empty RX Buffer
    while (this->available()) {
        this->read();
    }

    // calculate CRC
    command[commandLen - 1] = this->check_crc_(command, commandLen);

    this->write_array(command, commandLen);
    this->flush();

    if(response == nullptr) {
        return true;
    }

    return this->read_array(response, responseLen);
}

void CM1106xComponent::dump_config() {
    ESP_LOGCONFIG(TAG, "CM1106x:");
#ifdef USE_SENSOR
    if (this->co2_sensor_ != nullptr)
        LOG_SENSOR("  ", "CO2", this->co2_sensor_);
#endif
#ifdef USE_SWITCH
    if (this->co2_calibration_switch_ != nullptr)
        LOG_SWITCH("  ", "CO2 Calibration", this->co2_calibration_switch_);
#endif
}

}  // namespace cm1106x
}  // namespace esphome
