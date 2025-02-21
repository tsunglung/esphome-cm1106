#include "cm1106x_switch.h"


namespace esphome {
namespace cm1106x {


void CM1106xSwitch::write_state(bool state) {
  this->publish_state(state);
  this->parent_->set_co2_calibration(state);
}

}  // namespace cm1106x
}  // namespace esphome