#pragma once

#include "esphome/components/button/button.h"
#include "../cm1106x.h"

namespace esphome {
namespace cm1106x {

class CM1106xButton : public switch_::Switch, public Parented<CM1106xComponent> {
  public:
  CM1106xButton() = default;

 protected:
  void press_action() override;
};

}  // namespace cm1106x
}  // namespace esphome