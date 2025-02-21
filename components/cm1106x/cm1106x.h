#pragma once

#include <iomanip>
#include <map>
#include "esphome/components/uart/uart.h"
#include "esphome/core/component.h"
#include "esphome/core/defines.h"
#include "esphome/core/helpers.h"
#ifdef USE_SENSOR
#include "esphome/components/sensor/sensor.h"
#endif
#ifdef USE_SWITCH
#include "esphome/components/switch/switch.h"
#endif
#include "esphome/components/uart/uart.h"

namespace esphome {
namespace cm1106x {

class CM1106xComponent : public PollingComponent, public uart::UARTDevice {
  public:
  CM1106xComponent() = default;

#ifdef USE_SENSOR
  SUB_SENSOR(co2)
#endif

#ifdef USE_SWITCH
  SUB_SWITCH(co2_calibration)
#endif

  void set_co2_calibration(uint16_t ppm);
  uint8_t check_crc_(uint8_t* response, size_t len);
  bool send_command(uint8_t *command, size_t commandLen, uint8_t *response, size_t responseLen);
  int16_t get_co2_ppm();

  void dump_config() override;
  void setup() override;
  void update() override;
  void loop() override;

  float get_setup_priority() const override;

  protected:
};

}  // namespace cm1106x
}  // namespace esphome
