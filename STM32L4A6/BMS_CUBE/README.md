# Modular BMS Board
<a href="https://docs.google.com/document/d/1XrpsYSUMBOdzRiZzQP9mPXzkb9d297Olm9vWyCTE074/edit#"> More info </a>
<br>

## Purpose
---
- Read battery voltage and temperature from ADC
  - Send voltage data upstream
- Relay data through module from upstream and downstream SPIs
- Send heartbeat upstream

## Tasks
---
- Consumes:
  - BmsCanRxQ: including data from each battery modules inside bps
- Feeds:
  - BmsCanTxQ
  - CanTxQ
- Output:
  - FanPowerSwitch
- Function:
  - Handshake with BPS (via BpsCanRxQ and BpsCanTxQ)
  - Monitor the BPS heartbeats on BpsCanRxQ
  - Collects status data (ex. Voltage, current, temperature) from BmsCanRxQ
  - Sends status data to the CanTxQ
  - If the temperature data reach the threshold values, turn on fans’ power

## Pinout
---
- `LED_R`, `LED_G`, `LED_B`: GPIO_OUTPUT, LED output
- `CAN1_TX`, `CAN1_RX`: 
  - CAN
  - Handshake and communicate with BPS
- `IF_ALERT`: 
  - GPIO_INPUT, **Interrupt**
  - BQ76925_IF
- `I2C1_SDA`, `I2C1_SCL`: 
  - I2C, **Interrupt**
  - BQ76925_IF
- `ADC_Tx (x from 1 to 5)`:
  - ADC1, **DMA1 Channel1**, Medium
  - Thermistor
- `ADC_VREF`, `ADC_VC`, `ADC_VI`:
  - ADC2, **DMA1 Channel2**, Medium
  - BQ76925_IF
  - range from -0.3 ~ 3.6 v
  - VC: voltage
  - VI: peak current
  - VREF: refrence voltage for ADC
- `USART2`: UART via USB

## BQ76925
---
<a href="http://www.ti.com/lit/ds/symlink/bq76925.pdf">Datasheet</a>
- I2C address
  - `ADDRESS[6:0] = (I2C_GROUP_ADDR[3:0] << 3) + REG_ADDR[4:0]`
- Register map
  - 8.6 pg21

## C Files
---
- **BSSR_LED.c/.h**: LED controller
- **BSSR_CAN.c/.h**: CAN driver
- **BSSR_I2C.c/.h**: I2C driver
- **BSSR_USART.c/.h**: Usart driver
- **THEMISTER_READER.c/.h**
- **BQ76925.c/.h**: BQ76925 controller
