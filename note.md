---
output:
  custom_document:
    path: output.html
    pandoc_args: [
    "--self-contained",
    # "--number-sections", # if you like it numbered
    # "--reference-docx=C:/Path/To/Reference.docx",
    "-M","css=~/.pandoc/pandoc_misc/github_css/github.css", # this does not work
    "--read=markdown+east_asian_line_breaks+emoji",
    "--toc",
    "-t","html"
    ]
---

# FRDM-K64F
- [platform page on mbed.org](https://developer.mbed.org/platforms/FRDM-K64F/)

## Board overview

|           [(K64F USB)]           | [RJ45] | [(mbed USB)] |                 [Reset button #1]                  |
|:--------------------------------:|:------:|:------------:|:--------------------------------------------------:|
|  **[[Headers #1]](#headers-1)**  |        |              |           **[[Headers #3]](#headers-3)**           |
|                                  |        |              |                                                    |
|  **[[Headers #2]](#headers-2)**  |        |              |           **[[Headers #4]](#headers-4)**           |
|                                  |        |              |        **[[Accelerometer]](#accelerometer)**        |
|                                  |        |              |      **[[Wifi Module]](#wifi-module-headers)**      |
|   **[[RGB LED]](#user-leds)**    |        |              | **[[Bluetooth Module]](#bluetooth-module-headers)** |
| **[[Button #3]](#user-buttons)** |        |              |          **[[Button #2]](#user-buttons)**          |
|   **[[uSD socket]](#microsd)**   |        |              |                                                    |

## pins on left headers
### [Headers #1]

| function | pin | --- | function | pin |
|----------|-----|-----|----------|-----|
| NC       | NA  | --- | NC       | NA  |
| 3.3V     | NA  | --- | NC       | NA  |
| RESET    | NA  | --- | NC       | NA  |
| 3.3V     | NA  | --- | NC       | NA  |
| 5V       | NA  | --- | NC       | NA  |
| GND      | NA  | --- | NC       | NA  |
| GND      | NA  | --- | NC       | NA  |
| VIN      | NA  | --- | NC       | NA  |

### [Headers #2]

| function | PinName | --- | function              | PinName |
|----------|---------|-----|-----------------------|---------|
| A0       | PTB2    | --- | ADC0_D~ifferential~P1 | ?       |
| A1       | PTB3    | --- | ADC0_D~ifferential~M1 | ?       |
| A2       | PTB10   | --- | ADC1_D~ifferential~P1 | ?       |
| A3       | PTB11   | --- | ADC1_D~ifferential~M1 | ?       |
| A4 \*    | PTC11   | --- | PTB20                 | PTB20   |
| A5 \*    | PTC10   | --- | DAC_OUT               | ?       |

- Pins with * has PWM capability

## pins on right headers
### [Headers #3]

| function              | PinName | --- | function | PinName |
|-----------------------|---------|-----|----------|---------|
| NC                    | NA      | --- | SCL      | PTE24   |
| ADC1_S~ingle~E~nd~18  | PTB2    | --- | SDA      | PTE25   |
| NC                    | NA      | --- | AREF     | NA      |
| ADC1_D~ifferential~M0 | ?       | --- | GND      | NA      |
| ADC1_D~ifferential~P0 | ?       | --- | D13 \*   | PTD1    |
| NC                    | NA      | --- | D12 \*   | PTD3    |
| ADC0_D~ifferential~M0 | ?       | --- | D11 \*   | PTD2    |
| ADC0_D~ifferential~P0 | ?       | --- | D10 \*   | PTD0    |
| GND                   | NA      | --- | D9  \*   | PTC4    |
| PTE26                 | PTE26   | --- | D8  \*   | PTC12   |

- Pins with * has PWM capability

### [Headers #4]

| function               | PinName | --- | function | PinName |
|------------------------|---------|-----|----------|---------|
| I2S_RXD                | PTC5    | --- | D7 \*    | PTC3    |
| I2S_RX_F~rame~S~elect~ | PTC7    | --- | D6 \*    | PTC2    |
| I2S_SOF_OUT            | PTC0    | --- | D5 \*    | PTA2    |
| I2S_RX_B~it~CL~oc~K    | PTC9    | --- | D4       | PTB23   |
| I2S_MCLK               | PTC8    | --- | D3 \*    | PTA1    |
| I2S_TXD                | PTC1    | --- | D2       | PTB9    |
| I2S_TX_F~rame~S~elect~ | PTB19   | --- | D1       | PTC17   |
| I2S_TX_B~it~CL~oc~K    | PTB18   | --- | D0       | PTC16   |

- Pins with * has PWM capability

## pins on other headers
### [Wifi Module Headers]

| # | function | PinName | # | function | PinName |
|---|----------|---------|---|----------|---------|
| 1 | GND      | NA      | 2 | 3.3V     | NA      |
| 3 | CE       | PTC12 * | 4 | CS       | PTD4    |
| 5 | SCLK     | PTD5    | 6 | MOSI     | PTD6    |
| 7 | MISO     | PTD7    | 8 | IRQ      | PTC18   |

- **PinName for CE pin changes by board revision; this table shows D1 revision**

### [Bluetooth Module Headers]

| # | function | PinName |
|---|----------|---------|
| 1 | 3.3V     | NA      |
| 2 | GND      | NA      |
| 3 | UART RX  | PTC14   |
| 4 | UART TX  | PTC15   |

## connections to onboard features
### [Accelerometer]

| function | PinName |
|----------|---------|
| SCL      | PTE24   |
| SDA      | PTE25   |
| INT1     | PTC6    |
| INT2     | PTC13   |

### [microSD]

| function | PinName |
|----------|---------|
| MOSI     | PTE3    |
| MISO     | PTE1    |
| SCLK     | PTE2    |
| CS       | PTE4    |
| DETECT   | PTE6    |

### [user LEDs]

| function | PinName |
|----------|---------|
| R        | PTB22   |
| G        | PTE26   |
| B        | PTB21   |

### [user buttons]

| function | PinName |
|----------|---------|
| SW2      | PTC6    |
| SW3      | PTA3    |

## MCU Features
- Kinetis MK64FN1M0VLL12 in 100LQFP
- Performance
    - ARM® Cortex™-M4 32-bit core with DSP instructions and Floating Point Unit (FPU)
    - 120 MHz max CPU frequency
- Memories and memory interfaces
    - 1024 KB program flash memory
    - 256 KB RAM
    - FlexBus external bus interface
- System peripherals
    - Multiple low-power modes, low-leakage wake-up unit
    - 16-channel DMA controller
- Clocks
    - 3x Internal Reference Clocks: 32KHz, 4MHz and 48MHz
    - 2x Crystal inputs: 3-32MHz (XTAL0) and 32kHz (XTAL32/RTC)
    - PLL and FL
- Analog modules
    - 2x 16-bit SAR ADCs up 800ksps (12-bit mode)
    - 2x 12-bit DACs
    - 3x Analog comparators
    - Voltage reference 1.13V
- Communication interfaces
    - 1x 10/100 Mbit/s Ethernet MAC controller with MII/RMII interface IEEE1588 capable
    - 1x USB 2.0 Full-/Low-Speed Device/Host/OTG controller with embedded 3.3V/120mA Vreg, and USB device Crystal-less operation
    - 1x Controller Area Network (CAN) module
    - 3x SPI modules
    - 3x I2C modules. Support for up to 1 Mbit/s
    - 6x UART modules
    - 1x Secure Digital Host Controller (SDHC)
    - 1x I2S module
- Timers
    - 2x 8-channel Flex-Timers (PWM/Motor control)
    - 2x 2-channel FlexTimers (PWM/Quad decoder)
    - 32-bit PITs and 16-bit low-power timers
    - Real-Time Clock (RTC)
    - Programmable delay block
- Security and integrity modules
    - Hardware CRC and random-number generator modules
    - Hardware encryption supporting DES, 3DES, AES, MD5, SHA-1 and SHA-256 algorithms
- Operating Characteristics
    - Voltage range: 1.71 to 3.6 V
    - Flash write voltage range: 1.71 to 3.6 V

## Board Features
- Onboard Components
    - FXOS8700CQ - 6-axis combo Sensor Accelerometer and Magnetometer
        - [link to driver page](https://developer.mbed.org/users/AswinSivakumar/code/FXOS8700/)
    - 2 user push-buttons
    - RGB LED
- Connectivity
    - USB full-/low-speed On-the-Go/Host/Device controller with on-chip transceiver,
      5 V to 3.3 V regulator and micro-USB connector
    - Ethernet 10/100 controller with on-board transceiver and RJ45 connector
    - up to 5x UARTs, 2x SPIs, 2x I2Cs and 1x CAN connected to Headers (multiplexed peripherals)
- Extensions
    - Micro SD-Card Socket
    - Headers compatible with Arduino R3 shields (32-pins / outter row)
    - Headers for proprietary shields (32-pins / inner row)
- Analog and Digital IOs (multiplexed peripherals)
    - up to two ADC 16-bit resolution with 28 Analog I/O Pins connected to Headers
    - up to three timers with 18 PWM signals accessible from Headers
    - up to six Comparator Inputs or one DAC Output
    - up to 40 MCU I/O Pins connected to Headers (3.3v, 4mA each, 400mA max total)
- Board power-supply options (onboard 5 to 3.3V regulator)
    - USB Debug 5V
    - USB Target 5V
    - 5-9V Vin on Arduino headers
    - 5V PWR input
    - Coin-cell 3.3V
- Integrated OpenSDA USB Debug and Programming adapter
    - Several industry standard Debug interfaces (PEmicro, CMSIS-DAP, JLink)
    - Drag-n-drop MSD Flash-programming
    - Virtual USB to Serial Port
- Form factor: 3.2” x 2.1” / 81mm x 53mm
- Software Development Tools
    - mbed HDK & SDK enabled
    - Online development tools
    - Easy to use C/C++ SDK
    - Lots of published libraries and projects
    - Alternate Offline options NXP free KDS (compiler toolchain) and KSDK library/examples
- Supplier website: http://www.nxp.com/frdm-k64F

# FRDM-K66F
- [platform page on mbed.org](https://developer.mbed.org/platforms/FRDM-K66F/)

## Board overview

|            [(K66F USB)]            | [RJ45] | [(mbed USB)] |                  [Reset button #1]                   |
|:----------------------------------:|:------:|:------------:|:----------------------------------------------------:|
|  **[[Headers #1]](#headers-1-1)**  |        |              |           **[[Headers #3]](#headers-3-1)**           |
|                                    |        |              |                                                      |
|  **[[Headers #2]](#headers-2-1)**  |        |              |           **[[Headers #4]](#headers-4-1)**           |
|                                    |        |              |        **[[Accelerometer]](#accelerometer-1)**        |
|                                    |        |              |      **[[Wifi Module]](#wifi-module-headers-1)**      |
|   **[[Gyroscope]](#gyroscope)**    |        |              |       **[[Audio Codec]](#audio-codec-da7212)**       |
|   **[[RGB LED]](#user-leds-1)**    |        |              | **[[Bluetooth Module]](#bluetooth-module-headers-1)** |
| **[[Button #3]](#user-buttons-1)** |        |              |          **[[Button #2]](#user-buttons-1)**          |
|   **[[uSD socket]](#microsd-1)**   |        |              |                                                      |

## pins on left headers
### [Headers #1]

| function | pin | --- | function | pin   |
|----------|-----|-----|----------|-------|
| NC       | NA  | --- | PTA27    | PTA27 |
| 3.3V     | NA  | --- | PTA26    | PTA26 |
| RESET    | NA  | --- | PTA4     | PTA4  |
| 3.3V     | NA  | --- | PTA6     | PTA6  |
| 5V       | NA  | --- | PTA7     | PTA7  |
| GND      | NA  | --- | PTA8     | PTA8  |
| GND      | NA  | --- | PTA9     | PTA9  |
| VIN      | NA  | --- | PTA1     | PTA1  |

### [Headers #2]

| function | PinName | --- | function              | PinName |
|----------|---------|-----|-----------------------|---------|
| A0       | PTB7    | --- | ADC1_D~ifferential~P0 | ?       |
| A1       | PTB6    | --- | ADC1_D~ifferential~M0 | ?       |
| A2       | PTB5    | --- | ADC0_S~ingle~E~nd~16  | ?       |
| A3       | PTB4    | --- | ADC1_S~ingle~E~nd~16  | ?       |
| A4 \*    | PTC3    | --- | PTB23                 | PTB23   |
| A5 \*    | PTB2    | --- | DAC_OUT               | ?       |

- Pins with * has PWM capability

## pins on right headers
### [Headers #3]

| function              | PinName | --- | function | PinName |
|-----------------------|---------|-----|----------|---------|
| PTD12                 | PTD12   | --- | SCL      | PTC10   |
| PTD13                 | PTD13   | --- | SDA      | PTC11   |
| PTE25                 | PTE25   | --- | AREF     | NA      |
| PTE24                 | PTE24   | --- | GND      | NA      |
| PTC0                  | PTC0    | --- | D13      | PTD1    |
| PTB11                 | PTB11   | --- | D12      | PTD3    |
| PTB10                 | PTB10   | --- | D11      | PTD2    |
| ADC1_S~ingle~E~nd~23  | ?       | --- | D10 \*   | PTD0    |
| ADC0_D~ifferential~M0 | ?       | --- | D9  \*   | PTA19   |
| ADC1_S~ingle~E~nd~18  | ?       | --- | D8  \*   | PTB18   |

- Pins with * has PWM capability

### [Headers #4]

| function               | PinName | --- | function | PinName |
|------------------------|---------|-----|----------|---------|
| I2S_RXD                | PTE7    | --- | D7       | PTA25   |
| I2S_RX_F~rame~S~elect~ | PTE8    | --- | D6 \*    | PTC2    |
| I2S_SOF_OUT            | PTC7    | --- | D5 \*    | PTC5    |
| I2S_RX_B~it~CL~oc~K    | PTC9    | --- | D4 \*    | PTC12   |
| I2S_MCLK               | PTC6    | --- | D3 \*    | PTC8    |
| I2S_TXD                | PTC1    | --- | D2       | PTC16   |
| I2S_TX_F~rame~S~elect~ | PTE11   | --- | D1       | PTC4    |
| I2S_TX_B~it~CL~oc~K    | PTE12   | --- | D0       | PTC3    |

- Pins with * has PWM capability

## pins on other headers
### [Wifi Module Headers]

| # | function | PinName | # | function | PinName |
|---|----------|---------|---|----------|---------|
| 1 | GND      | NA      | 2 | 3.3V     | NA      |
| 3 | CE       | PTB20   | 4 | CS       | PTD4    |
| 5 | SCLK     | PTD5    | 6 | MOSI     | PTD6    |
| 7 | MISO     | PTD7    | 8 | IRQ      | PTC18   |

### [Bluetooth Module Headers]

| # | function | PinName |
|---|----------|---------|
| 1 | 3.3V     | NA      |
| 2 | GND      | NA      |
| 3 | UART RX  | PTC14   |
| 4 | UART TX  | PTC15   |

## connections to onboard features
### [Accelerometer]

| function | PinName |
|----------|---------|
| SDA      | PTD9    |
| SCL      | PTD8    |
| INT1     | PTC6    |
| INT2     | PTC13   |

### [Gyroscope]

| function | PinName |
|----------|---------|
| SDA      | PTD9    |
| SCL      | PTD8    |
| INT1     | PTA29   |
| INT2     | PTA28   |

### [Audio codec DA7212]

| function(DA7212/K66F) | PinName                 |
|-----------------------|-------------------------|
| SDA                   | PTC11                   |
| SCL                   | PTC10                   |
| BCLK                  | ***1** [J37](#j37-bclk) |
| WCLK                  | ***2** [J38](#j38-wclk) |
| DATIN/TXD             | PTC1                    |
| DATOUT/RXD            | PTE7                    |

#### J37: BCLK

| short  | function    | PinName |
|--------|-------------|---------|
| 2-3(*) | I2S_TX_BCLK | PTE12   |
| 1-2    | I2S_RX_BCLK | PTE9    |

#### J38: WCLK

| short  | function    | PinName |
|--------|-------------|---------|
| 2-3(*) | I2S_TX_WCLK | PTE11   |
| 1-2    | I2S_RX_WCLK | PTE8    |

### [microSD]

| SPI mode | SD mode | PinName |
|----------|---------|---------|
|          | DAT2    | PTE5    |
| CS       | CD/DAT3 | PTE4    |
| MOSI     | CMD     | PTE3    |
| SCLK     | CLK     | PTE2    |
| MISO     | DAT0    | PTE1    |
|          | DAT1    | PTE0    |
| DETECT   |         | PTD10   |

### [user LEDs]

| function | PinName |
|----------|---------|
| R        | PTC9    |
| G        | PTE6    |
| B        | PTA11   |

### [user buttons]

| function | PinName |
|----------|---------|
| SW2      | PTA4    |
| SW3      | PTA13   |

## MCU Features
- Kinetis MK66FN2M0VMD18 in 144BGA package
- ARM® Cortex™-M4 32-bit core, with DSP instructions
- 180 MHz max CPU frequency
- 2 MB program flash memory
- 256 KB RAM
- FlexBus external bus interface and SDRAM controller
- Multiple low-power modes
- 1x Internal Reference Clocks, 2x Crystal inputs, 1x real-time clock
- 6x UART modules including 1 low-power TX/RX
- 2x CAN modules
- 3x SPI modules
- 4x I2C modules
- 1x I2S module
- 1x USB high-speed OTG, 1x USB full-speed OTG
- 1x Ethernet MAC controller with MII/RMII interface IEEE1588 capable
- Secure Digital Host Controller
- 2x 16-bit ADC, 2x 12-bit DAC
- 11x Timers
- Low-power hardware touch sensor interface (TSI)
- Hardware random-number generator
- Hardware encryption supporting DES, AES and SHA algorithms
- Advanced flash security


## Board Features
- Onboard Components
    - FXOS8700CQ - Accelerometer and Magnetometer
        - [link to driver page](https://developer.mbed.org/users/AswinSivakumar/code/FXOS8700/)
    - FXAS21002 - Gyroscope
        - [link to driver page](https://developer.mbed.org/users/AswinSivakumar/code/FXAS21002/)
    - 2 user push-buttons
    - RGB LED
- Connectivity
    - Dual role High-speed USB interface with micro-B USB connector
    - Ethernet 10/100 controller with on-board transceiver and RJ45 connector
    - up to 5x UARTs, 3x SPIs, 3x I2Cs, 1xI2S and 2xCANs connected to Headers (multiplexed peripherals)
- Extensions
    - Micro SD-Card Socket
    - Headers compatible with Arduino R3 shields (32-pins / outter row)
    - Headers for proprietary shields (32-pins / inner row)
    - Optional header for add-on RF module: RF24L01+ Nordic 2.4 GHz Radio
    - Optional header for add-on Bluetooth module: JY-MCU BT Board V1.05 BT
- Analog and Digital IOs (multiplexed peripherals)
    - 2x ADC 16-bit resolution with 29 Analog I/O Pins connected to Headers
    - up to 6x timers with 37 PWM signals accessible from Headers
    - up to 4x Comparator Inputs or 2x DAC outputs
    - up to 46 MCU I/O Pins connected to Headers (3.3v, 4mA each, 400mA max total)
- Audio
    - Digital MEMS microphone
    - Auxiliary input jack
    - Headset/Analog microphone jack
    - Two optional input for analogue microphone
- Board power-supply options (onboard 5 to 3.3V regulator)
    - USB Debug 5V
    - USB Target 5V
    - 5-9V Vin on Arduino headers
    - 5V PWR input
    - Coin-cell 3.3V
- Integrated OpenSDA USB Debug and Programming adapter
    - Several industry standard Debug interfaces (PEmicro, CMSIS-DAP, JLink)
    - Drag-n-drop MSD Flash-programming
    - Virtual USB to Serial Port
- Form factor: 3.2” x 2.1” / 81mm x 53mm
- Software Development Tools
    - mbed HDK & SDK enabled
    - Online development tools
    - Easy to use C/C++ SDK
    - Lots of published libraries and projects
    - Alternate Offline options NXP free KDS (compiler toolchain) and KSDK library/examples
- Supplier website: http://www.nxp.com/frdm-k66f

# K66F pin config from datasheet P184~; Picking up I2S related pins

| 144 LQFP | **144 MAP BGA** | Pin Name          | Default             | ~ | **ALT4**          | ALT5              | **ALT6**      |
|----------|-----------------|-------------------|---------------------|---|-------------------|-------------------|---------------|
| 10       | F4              | **PTE7**          | DISABLED            |   | **I2S0_RXD0**     |                   | FTM3_CH2      |
| 11       | F3              | **PTE8**          | DISABLED            |   | **I2S0_RX_FS**    | LPUART0_TX        | FTM3_CH3      |
| 14       | G4              | **PTE11**         | DISABLED            |   | **I2S0_TX_FS**    | LPUART0_RTS_b     | FTM3_CH6      |
| 15       | G3              | **PTE12**         | DISABLED            |   | **I2S0_TX_BCLK**  |                   | FTM3_CH7      |
| 104      | B11             | **PTC1**/LLWU_P6  | ADC0_SE15/TSI0_CH14 |   | FTM0_CH0          | FB_AD13/SDRAM_A21 | **I2S0_TXD0** |
| 111      | C8              | **PTC6**/LLWU_P10 | CMP0_IN0            |   | I2S0_RX_BCLK      | FB_AD9/SDRAM_A17  | **I2S0_MCLK** |
| 114      | D7              | **PTC9**          | ADC1_SE5b/CMP0_IN3  |   | **I2S0_RX_BCLK**  | FB_AD6/SDRAM_A14  | FTM2_FLT0     |

# K64F/K66F I2S peripheral description from datasheet

```
+------------+---------------------------------------------------+-----------+--------------------+-------------+
| Absolute \ | Register name                                     | Width\    | Access             | Reset value |
| address \  |                                                   | (in bits) |                    |             |
| (hex)      |                                                   |           |                    |             |
+============+===================================================+===========+====================+=============+
| 4002_F000  | SAI Transmit Control Register (I2S0_TCSR)         | 32        | R/W                | 0000_0000h  |
+------------+---------------------------------------------------+-----------+--------------------+-------------+
| 4002_F004  | SAI Transmit Configuration 1 Register (I2S0_TCR1) | 32        | R/W                | 0000_0000h  |
+------------+---------------------------------------------------+-----------+--------------------+-------------+
| 4002_F008  | SAI Transmit Configuration 2 Register (I2S0_TCR2) | 32        | R/W                | 0000_0000h  |
+------------+---------------------------------------------------+-----------+--------------------+-------------+
| 4002_F00C  | SAI Transmit Configuration 3 Register (I2S0_TCR3) | 32        | R/W                | 0000_0000h  |
+------------+---------------------------------------------------+-----------+--------------------+-------------+
| 4002_F010  | SAI Transmit Configuration 4 Register (I2S0_TCR4) | 32        | R/W                | 0000_0000h  |
+------------+---------------------------------------------------+-----------+--------------------+-------------+
| 4002_F014  | SAI Transmit Configuration 5 Register (I2S0_TCR5) | 32        | R/W                | 0000_0000h  |
+------------+---------------------------------------------------+-----------+--------------------+-------------+
| 4002_F020  | SAI Transmit Data Register (I2S0_TDR0)            | 32        | W (always reads 0) | 0000_0000h  |
+------------+---------------------------------------------------+-----------+--------------------+-------------+
| 4002_F024  | SAI Transmit Data Register (I2S0_TDR1)            | 32        | W (always reads 0) | 0000_0000h  |
+------------+---------------------------------------------------+-----------+--------------------+-------------+
| 4002_F040  | SAI Transmit FIFO Register (I2S0_TFR0)            | 32        | R                  | 0000_0000h  |
+------------+---------------------------------------------------+-----------+--------------------+-------------+
| 4002_F044  | SAI Transmit FIFO Register (I2S0_TFR1)            | 32        | R                  | 0000_0000h  |
+------------+---------------------------------------------------+-----------+--------------------+-------------+
| 4002_F060  | SAI Transmit Mask Register (I2S0_TMR)             | 32        | R/W                | 0000_0000h  |
+------------+---------------------------------------------------+-----------+--------------------+-------------+
| 4002_F080  | SAI Receive Control Register (I2S0_RCSR)          | 32        | R/W                | 0000_0000h  |
+------------+---------------------------------------------------+-----------+--------------------+-------------+
| 4002_F084  | SAI Receive Configuration 1 Register (I2S0_RCR1)  | 32        | R/W                | 0000_0000h  |
+------------+---------------------------------------------------+-----------+--------------------+-------------+
| 4002_F088  | SAI Receive Configuration 2 Register (I2S0_RCR2)  | 32        | R/W                | 0000_0000h  |
+------------+---------------------------------------------------+-----------+--------------------+-------------+
| 4002_F08C  | SAI Receive Configuration 3 Register (I2S0_RCR3)  | 32        | R/W                | 0000_0000h  |
+------------+---------------------------------------------------+-----------+--------------------+-------------+
| 4002_F090  | SAI Receive Configuration 4 Register (I2S0_RCR4)  | 32        | R/W                | 0000_0000h  |
+------------+---------------------------------------------------+-----------+--------------------+-------------+
| 4002_F094  | SAI Receive Configuration 5 Register (I2S0_RCR5)  | 32        | R/W                | 0000_0000h  |
+------------+---------------------------------------------------+-----------+--------------------+-------------+
| 4002_F0A0  | SAI Receive Data Register (I2S0_RDR0)             | 32        | R                  | 0000_0000h  |
+------------+---------------------------------------------------+-----------+--------------------+-------------+
| 4002_F0A4  | SAI Receive Data Register (I2S0_RDR1)             | 32        | R                  | 0000_0000h  |
+------------+---------------------------------------------------+-----------+--------------------+-------------+
| 4002_F0C0  | SAI Receive FIFO Register (I2S0_RFR0)             | 32        | R                  | 0000_0000h  |
+------------+---------------------------------------------------+-----------+--------------------+-------------+
| 4002_F0C4  | SAI Receive FIFO Register (I2S0_RFR1)             | 32        | R                  | 0000_0000h  |
+------------+---------------------------------------------------+-----------+--------------------+-------------+
| 4002_F0E0  | SAI Receive Mask Register (I2S0_RMR)              | 32        | R/W                | 0000_0000h  |
+------------+---------------------------------------------------+-----------+--------------------+-------------+
| 4002_F100  | SAI MCLK Control Register (I2S0_MCR)              | 32        | R/W                | 0000_0000h  |
+------------+---------------------------------------------------+-----------+--------------------+-------------+
| 4002_F104  | SAI MCLK Divide Register (I2S0_MDR)               | 32        | R/W                | 0000_0000h  |
+------------+---------------------------------------------------+-----------+--------------------+-------------+
```

## 54.4 Functional description
This section provides a complete functional description of the block.

### 54.4.1 SAI clocking
The SAI clocks include:
* The audio master clock
* The bit clock
* The bus clock

#### 54.4.1.1 Audio master clock
The audio master clock is used to generate the bit clock when the receiver or transmitter
is configured for an internally generated bit clock. The transmitter and receiver can
independently select between the bus clock and up to three audio master clocks to
generate the bit clock.

Each SAI peripheral can control the input clock selection, pin direction and divide ratio
of one audio master clock. The input clock selection and pin direction cannot be altered if
an SAI module using that audio master clock has been enabled. The MCLK divide ratio
can be altered while an SAI is using that master clock, although the change in the divide
ratio takes several cycles. `MCR[DUF]` can be polled to determine when the divide ratio
change has completed.

The audio master clock generation and selection is chip-specific. Refer to chip-specific
clocking information about how the audio master clocks are generated. A typical
implementation appears in the following figure.

The MCLK fractional clock divider uses both clock edges from the input clock to
generate a divided down clock that will approximate the output frequency, but without
creating any new clock edges. Configuring FRACT and DIVIDE to the same value will
result in a divide by 1 clock, while configuring FRACT higher than DIVIDE is not
supported. The duty cycle can range from 66/33 when FRACT is set to one less than
DIVIDE down to 50/50 for integer divide ratios, and will approach 50/50 for large noninteger
divide ratios. There is no cycle to cycle jitter or duty cycle variance when the
divide ratio is an integer or half integer, otherwise the divider output will oscillate
between the two divided frequencies that are the closest integer or half integer divisors of
the divider input clock frequency. The maximum jitter is therefore equal to half the
divider input clock period, since both edges of the input clock are used in generating the
divided clock.

#### 54.4.1.2 Bit clock
The SAI transmitter and receiver support asynchronous free-running bit clocks that can
be generated internally from an audio master clock or supplied externally. There is also
the option for synchronous bit clock and frame sync operation between the receiver and
transmitter or between multiple SAI peripherals.

Externally generated bit clocks must be:
* Enabled before the SAI transmitter or receiver is enabled
* Disabled after the SAI transmitter or receiver is disabled and completes its current frames

If the SAI transmitter or receiver is using an externally generated bit clock in
asynchronous mode and that bit clock is generated by an SAI that is disabled in stop
mode, then the transmitter or receiver should be disabled by software before entering stop
mode. This issue does not apply when the transmitter or receiver is in a synchronous
mode because all synchronous SAIs are enabled and disabled simultaneously.

#### 54.4.1.3 Bus clock
The bus clock is used by the control and configuration registers and to generate
synchronous interrupts and DMA requests.

#### 54.4.2 SAI resets
The SAI is asynchronously reset on system reset. The SAI has a software reset and a
FIFO reset.

#### 54.4.2.1 Software reset
The SAI transmitter includes a software reset that resets all transmitter internal logic,
including the bit clock generation, status flags, and FIFO pointers. It does not reset the
configuration registers. The software reset remains asserted until cleared by software.
The SAI receiver includes a software reset that resets all receiver internal logic, including
the bit clock generation, status flags and FIFO pointers. It does not reset the configuration
registers. The software reset remains asserted until cleared by software.

#### 54.4.2.2 FIFO reset
The SAI transmitter includes a FIFO reset that synchronizes the FIFO write pointer to the
same value as the FIFO read pointer. This empties the FIFO contents and is to be used
after `TCSR[FEF]` is set, and before the FIFO is re-initialized and TCSR[FEF] is cleared.

The FIFO reset is asserted for one cycle only.

The SAI receiver includes a FIFO reset that synchronizes the FIFO read pointer to the
same value as the FIFO write pointer. This empties the FIFO contents and is to be used
after the `RCSR[FEF]` is set and any remaining data has been read from the FIFO, and
before the `RCSR[FEF]` is cleared. The FIFO reset is asserted for one cycle only.

### 54.4.3 Synchronous modes
The SAI transmitter and receiver can operate synchronously to each other or
synchronously to other SAI peripherals.

#### 54.4.3.1 Synchronous mode
The SAI transmitter and receiver can be configured to operate with synchronous bit clock
and frame sync.

If the transmitter bit clock and frame sync are to be used by both the transmitter and receiver:
* The transmitter must be configured for asynchronous operation and the receiver for synchronous operation.
* In synchronous mode, the receiver is enabled only when both the transmitter and receiver are enabled.
* It is recommended that the transmitter is the last enabled and the first disabled.

If the receiver bit clock and frame sync are to be used by both the transmitter and receiver:
* The receiver must be configured for asynchronous operation and the transmitter for synchronous operation.
* In synchronous mode, the transmitter is enabled only when both the receiver and transmitter are both enabled.
* It is recommended that the receiver is the last enabled and the first disabled.

When operating in synchronous mode, only the bit clock, frame sync, and transmitter/
receiver enable are shared. The transmitter and receiver otherwise operate independently,
although configuration registers must be configured consistently across both the
transmitter and receiver.

#### 54.4.3.2 Multiple SAI Synchronous mode
Synchronous operation between multiple SAI peripherals is not supported on all devices.
This mode requires the source of the bit clock and frame sync to be configured for
asynchronous operation and the remaining users of the bit clock and frame sync to be
configured for synchronous operation.

Synchronous operation between multiple SAI transmitters or receivers also requires the
source of the bit clock and frame sync to be enabled for any of the synchronous
transmitters or receivers to also be enabled. It is recommended that the source of the bit
clock and frame sync is the last enabled and the first disabled.

When operating in synchronous mode, only the bit clock, frame sync, and transmitter/
receiver enable are shared. The separate SAI peripherals otherwise operate
independently, although configuration registers must be configured consistently across
both the transmitter and receiver.

### 54.4.4 Frame sync configuration
When enabled, the SAI continuously transmits and/or receives frames of data. Each
frame consists of a fixed number of words and each word consists of a fixed number of
bits. Within each frame, any given word can be masked causing the receiver to ignore
that word and the transmitter to tri-state for the duration of that word.

The frame sync signal is used to indicate the start of each frame. A valid frame sync
requires a rising edge (if active high) or falling edge (if active low) to be detected and the
transmitter or receiver cannot be busy with a previous frame. A valid frame sync is also
ignored (slave mode) or not generated (master mode) for the first four bit clock cycles
after enabling the transmitter or receiver.

The transmitter and receiver frame sync can be configured independently with any of the following options:
* Externally generated or internally generated
* Active high or active low
* Assert with the first bit in frame or asserts one bit early
* Assert for a duration between 1 bit clock and the first word length
* Frame length from 1 to 32 words per frame
* Word length to support 8 to 32 bits per word
* First word length and remaining word lengths can be configured separately
* Words can be configured to transmit/receive MSB first or LSB first

These configuration options cannot be changed after the SAI transmitter or receiver is enabled.

### 54.4.5 Data FIFO
Each transmit and receive channel includes a FIFO of size 8 × 32-bit. The FIFO data is
accessed using the SAI Transmit/Receive Data Registers.

#### 54.4.5.1 Data alignment
Data in the FIFO can be aligned anywhere within the 32-bit wide register through the use
of the First Bit Shifted configuration field, which selects the bit index (between 31 and 0)
of the first bit shifted.

Examples of supported data alignment and the required First Bit Shifted configuration are
illustrated in Figure 54-59 for LSB First configurations and Figure 54-60 for MSB First
configurations.

Figure 54-59. SAI first bit shifted, LSB first
Figure 54-60. SAI first bit shifted, MSB first

#### 54.4.5.2 FIFO pointers
When writing to a TDR, the WFP of the corresponding TFR increments after each valid
write. The SAI supports 8-bit, 16-bit and 32-bit writes to the TDR and the FIFO pointer
will increment after each individual write. Note that 8-bit writes should only be used
when transmitting up to 8-bit data and 16-bit writes should only be used when
transmitting up to 16-bit data.

Writes to a TDR are ignored if the corresponding bit of TCR3[TCE] is clear or if the
FIFO is full. If the Transmit FIFO is empty, the TDR must be written at least three bit
clocks before the start of the next unmasked word to avoid a FIFO underrun.
When reading an RDR, the RFP of the corresponding RFR increments after each valid
read. The SAI supports 8-bit, 16-bit and 32-bit reads from the RDR and the FIFO pointer
will increment after each individual read. Note that 8-bit reads should only be used when
receiving up to 8-bit data and 16-bit reads should only be used when receiving up to 16-
bit data.

Reads from an RDR are ignored if the corresponding bit of RCR3[RCE] is clear or if the
FIFO is empty. If the Receive FIFO is full, the RDR must be read at least three bit clocks
before the end of an unmasked word to avoid a FIFO overrun.

### 54.4.6 Word mask register
The SAI transmitter and receiver each contain a word mask register, namely TMR and
RMR, that can be used to mask any word in the frame. Because the word mask register is
double buffered, software can update it before the end of each frame to mask a particular
word in the next frame.

The TMR causes the Transmit Data pin to be tri-stated for the length of each selected
word and the transmit FIFO is not read for masked words.

The RMR causes the received data for each selected word to be discarded and not written
to the receive FIFO.

### 54.4.7 Interrupts and DMA requests
The SAI transmitter and receiver generate separate interrupts and separate DMA requests,
but support the same status flags. Asynchronous versions of the transmitter and receiver
interrupts are generated to wake up the CPU from stop mode.

#### 54.4.7.1 FIFO request flag
The FIFO request flag is set based on the number of entries in the FIFO and the FIFO
watermark configuration.

The transmit FIFO request flag is set when the number of entries in any of the enabled
transmit FIFOs is less than or equal to the transmit FIFO watermark configuration and is
cleared when the number of entries in each enabled transmit FIFO is greater than the
transmit FIFO watermark configuration.

The receive FIFO request flag is set when the number of entries in any of the enabled
receive FIFOs is greater than the receive FIFO watermark configuration and is cleared
when the number of entries in each enabled receive FIFO is less than or equal to the
receive FIFO watermark configuration.

The FIFO request flag can generate an interrupt or a DMA request.

#### 54.4.7.2 FIFO warning flag
The FIFO warning flag is set based on the number of entries in the FIFO.

The transmit warning flag is set when the number of entries in any of the enabled
transmit FIFOs is empty and is cleared when the number of entries in each enabled
transmit FIFO is not empty.

The receive warning flag is set when the number of entries in any of the enabled receive
FIFOs is full and is cleared when the number of entries in each enabled receive FIFO is
not full.

The FIFO warning flag can generate an Interrupt or a DMA request.

#### 54.4.7.3 FIFO error flag
The transmit FIFO error flag is set when the any of the enabled transmit FIFOs
underflow. After it is set, all enabled transmit channels repeat the last valid word read
from the transmit FIFO until `TCSR[FEF]` is cleared and the next transmit frame starts.

All enabled transmit FIFOs must be reset and initialized with new data before
`TCSR[FEF]` is cleared.

RCSR[FEF] is set when the any of the enabled receive FIFOs overflow. After it is set, all
enabled receive channels discard received data until `RCSR[FEF]` is cleared and the next
next receive frame starts. All enabled receive FIFOs should be emptied before
`RCSR[FEF]` is cleared.

The FIFO error flag can generate only an interrupt.

#### 54.4.7.4 Sync error flag
The sync error flag, `TCSR[SEF]` or `RCSR[SEF]`, is set when configured for an externally
generated frame sync and the external frame sync asserts when the transmitter or receiver
is busy with the previous frame. The external frame sync assertion is ignored and the
sync error flag is set. When the sync error flag is set, the transmitter or receiver continues
checking for frame sync assertion when idle or at the end of each frame.

The sync error flag can generate an interrupt only.

#### 54.4.7.5 Word start flag
The word start flag is set at the start of the second bit clock for the selected word, as
configured by the Word Flag register field.

The word start flag can generate an interrupt only.
