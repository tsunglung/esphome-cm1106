ESPHome CM1106x CO₂ Sensor Custom Component


example:

```
external_components:
  - source: github://tsunglung/esphome-pm2x05@master
    components: [ cm1106x ]


uart:
  - id: cm1106_uart
    rx_pin: 16
    tx_pin: 17
    baud_rate: 9600


cm1106x:
  id: cm1106x_id
  uart_id: cm1106_uart

sensor:
  - platform: cm1106x
    co2:
      name: "CO2 Sensor"
      id: co2sensor

switch:
  - platform: cm1106x
    name: "CO2 calibration"
    cm1106x_id: cm1106x_id
    id: calibration
    internal: true
```