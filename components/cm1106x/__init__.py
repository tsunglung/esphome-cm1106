import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart
from esphome.const import CONF_ID

CODEOWNERS = ["@tsunglung"]
MULTI_CONF = True
DEPENDENCIES = ["uart"]

CONF_CM1106X_ID = "cm1106x_id"

cm1106x_ns = cg.esphome_ns.namespace("cm1106x")
CM1106xComponent = cm1106x_ns.class_(
    "CM1106xComponent", cg.PollingComponent, uart.UARTDevice
)

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(CM1106xComponent)
        }
    )
    .extend(uart.UART_DEVICE_SCHEMA)
    .extend(cv.polling_component_schema("30s"))
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)
