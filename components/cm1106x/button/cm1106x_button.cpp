#include "cm1106x_button.h"


namespace esphome {
namespace cm1106x {


void CM1106xButton::press_action() {
  uint16_t state = 400;
  this->parent_->set_co2_calibration(state);
}

}  // namespace cm1106x
}  // namespace esphome