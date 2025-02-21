import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import button
from esphome.const import (
    ENTITY_CATEGORY_CONFIG
)
from .. import CONF_CM1106X_ID, CM1106xComponent, cm1106x_ns

DEPENDENCIES = ["cm1106x"]
AUTO_LOAD = ["button"]

CM1106xButton = cm1106x_ns.class_("CM1106xButton", button.Button)

CONFIG_SCHEMA = button.button_schema(
    CM1106xButton,
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
    btn = await button.new_button(config)
    await cg.register_parented(btn, config[CONF_CM1106X_ID])
    cg.add(var.set_co2_calibration_button(btn))
