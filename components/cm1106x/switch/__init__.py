import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import switch
from esphome.const import (
    DEVICE_CLASS_SWITCH,
    ENTITY_CATEGORY_CONFIG
)
from .. import CONF_CM1106X_ID, CM1106xComponent, cm1106x_ns

DEPENDENCIES = ["cm1106x"]
AUTO_LOAD = ["switch"]

CM1106xSwitch = cm1106x_ns.class_("CM1106xSwitch", switch.Switch)

CONFIG_SCHEMA = switch.switch_schema(
    CM1106xSwitch,
    device_class=DEVICE_CLASS_SWITCH,
    entity_category=ENTITY_CATEGORY_CONFIG,
).extend(
    cv.Schema(
        {
            cv.GenerateID(CONF_CM1106X_ID): cv.use_id(CM1106xComponent),
        }
    )
)


async def to_code(config):
    var = await cg.get_variable(config[CONF_CM1106X_ID])
    s = await switch.new_switch(config)
    await cg.register_parented(s, config[CONF_CM1106X_ID])
    cg.add(var.set_co2_calibration_switch(s))
