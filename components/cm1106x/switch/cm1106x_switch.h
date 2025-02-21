#pragma once

#include "esphome/components/switch/switch.h"
#include "../cm1106x.h"

namespace esphome {
namespace cm1106x {

class CM1106xSwitch : public switch_::Switch, public Parented<CM1106xComponent> {
  public:
  CM1106xSwitch() = default;

  protected:
    void write_state(bool state) override;
};

}  // namespace cm1106x
}  // namespace esphome