# FRDM-K64F
- [platform page on mbed.org](https://developer.mbed.org/platforms/FRDM-K64F/)

|   [(K64F USB)]   |      [RJ45]      |      [(mbed USB)]      | [Reset button #1] |
|:----------------:|:----------------:|:----------------------:|:-----------------:|
| **[Headers #1]** |                  |                        | **[Headers #3]**  |
|                  |                  |                        |                   |
| **[Headers #2]** |                  |                        | **[Headers #4]**  |
|                  |                  |  **[Accelerometer]**   |                   |
|                  |                  |   **[Wifi Module]**    |                   |
|                  |  **[RGB LED]**   | **[Bluetooth Module]** |                   |
| **[Button #3]**  | **[uSD socket]** |                        |  **[Button #2]**  |


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
| A5 \*    | PTBC10  | --- | DAC_OUT               | ?       |

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

### [connection to onboard Accelerometer]

| function | PinName |
|----------|---------|
| SCL      | PTE24   |
| SDA      | PTE25   |
| INT1     | PTC6    |
| INT2     | PTC13   |

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

### [connection to onboard microSD]

| function | PinName |
|----------|---------|
| MOSI     | PTE3    |
| MISO     | PTE1    |
| SCLK     | PTE2    |
| CS       | PTE4    |
| DETECT   | PTE6    |

### [connection to onboard user RGB LED]

| function | PinName |
|----------|---------|
| R        | PTB22   |
| G        | PTE26   |
| B        | PTB21   |

### [connection to onboard user buttons]

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
        - [link to driver page]()
    - 2 user push-buttons
    - RGB LED
- Connectivity
    - USB full-/low-speed On-the-Go/Host/Device controller with on-chip transceiver, 5 V to 3.3 V regulator and micro-USB connector
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
