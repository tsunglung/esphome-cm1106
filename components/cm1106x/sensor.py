import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import (
    CONF_ID,
    CONF_CO2,
    DEVICE_CLASS_CARBON_DIOXIDE,
    ICON_MOLECULE_CO2,
    STATE_CLASS_MEASUREMENT,
    UNIT_PARTS_PER_MILLION
)
from . import CM1106xComponent, CONF_CM1106X_ID

TYPES = [
    CONF_CO2,
]

CONFIG_SCHEMA = cv.All(
    cv.Schema(
        {
            cv.GenerateID(CONF_CM1106X_ID): cv.use_id(CM1106xComponent),
            cv.Optional(CONF_CO2): sensor.sensor_schema(
                unit_of_measurement=UNIT_PARTS_PER_MILLION,
                icon=ICON_MOLECULE_CO2,
                accuracy_decimals=0,
                device_class=DEVICE_CLASS_CARBON_DIOXIDE,
                state_class=STATE_CLASS_MEASUREMENT,
            )
        }
    ).extend(cv.COMPONENT_SCHEMA)
    .extend(cv.polling_component_schema("30s"))
)


async def to_code(config):
    hub = await cg.get_variable(config[CONF_CM1106X_ID])
    for key in TYPES:
        if sensor_config := config.get(key):
            sens = await sensor.new_sensor(sensor_config)
            cg.add(getattr(hub, f"set_{key}_sensor")(sens))
