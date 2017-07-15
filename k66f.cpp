/** MIT LICENSE
Copyright 2017 K4zuki(github.com/K4zuki)

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
documentation files (the "Software"), to deal in the Software without restriction, including without limitation the
rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit
persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the
Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#if 0
#if defined(TARGET_K66F)
// http://www.nxp.com/assets/documents/data/en/reference-manuals/K66P144M180SF5RMV2.pdf for details
#include "FrdmI2s.h"

#include "fsl_port.h"  //NOLINT
#include "fsl_sai.h"   //NOLINT

#define I2S_DF_WORDWIDTH 16
#define I2S_DF_SAMPLERATE 32000
#define I2S_DF_MASTERSLAVE I2S_MASTER
#define I2S_DF_STEREOMONO I2S_STEREO
#define I2S_DF_MUTED I2S_UNMUTED
#define I2S_DF_INTERRUPT_FIFO_LEVEL 4

#define I2S_MAX_DENOMINATOR 256
#define I2S_MAX_NUMERATOR 256
#define I2S_MAX_BITRATE_DIV 64

#define I2S_PCLK_RATE 12288000

FunctionPointer FrdmI2s::I2STXISR;
FunctionPointer FrdmI2s::I2SRXISR;

bool FrdmI2s::txisr;
bool FrdmI2s::rxisr;

FrdmI2s::FrdmI2s(PinName SerialData, PinName WordSelect, PinName BitClk, int rxtx)
    : _rxtx(rxtx), IoPin(SerialData), WclkPin(WordSelect), BclkPin(BitClk) {
    SIM->SCGC6 &= ~(SIM_SCGC6_I2S_MASK);
    SIM->SCGC6 |= SIM_SCGC6_I2S(1);

    NVIC_DisableIRQ(I2S0_Tx_IRQn);
    NVIC_DisableIRQ(I2S0_Rx_IRQn);

    fourwire = false;

    reg_write_err = 0;

    pin_setup();

    if (pin_setup_err != 0) {
        perror("I2S Pins incorrectly defined.");
    }
    _i2s_init();
    _i2s_set_rate(32000);
    defaulter();
}

FrdmI2s::~FrdmI2s() {
    NVIC_DisableIRQ(I2S0_Tx_IRQn);
    NVIC_DisableIRQ(I2S0_Rx_IRQn);

    pin_setup();
    update_config();
}

void FrdmI2s::defaulter() {
    // I2S0->TCSR |= 1u << 31;

    stop();

    frequency();
    wordsize();
    role();
    stereomono();
    set_interrupt_fifo_level(I2S_DF_INTERRUPT_FIFO_LEVEL);
    mute(MUTED);

    // NVIC_SetVector(I2S0_Tx_IRQn, (uint32_t)&_i2sisr);
    // NVIC_EnableIRQ(I2S0_Tx_IRQn);
}
void FrdmI2s::format(int _role, int mclk, int sample, int bit) {
    /*
    role -> pin out
    mclk/sample/bit -> MCLK freq, BCLK freq, WCLK freq, FIFO depth
    */
    role(_role);
    mclk_freq(mclk);
    frequency(sample);
    wordsize(bit);
}

void FrdmI2s::write(char buf[], int len) {
    if (_rxtx == TRANSMIT) {
        if (len > get_fifo_limit()) len = get_fifo_limit();
        if (len <= 0) return;
        int temp = 0;
        for (int i = 0; i < len; i += 4) {
            temp = 0;
            for (int j = 0; j < 4; j++) {
                temp |= int(buf[i + j]) << (j * 8);
            }
            I2S0->TDR[0] = temp;
        }
    }
}

void FrdmI2s::write(int buf[], int len) {
    if (_rxtx == TRANSMIT && _bit > 0) {
        if (len > get_fifo_limit()) {
            len = get_fifo_limit();
            printf("Trying to write too much data!\n\r");
        }
        if (len <= 0) return;
        uint32_t temp = 0;
        int increment = 32 / _bit;
        unsigned char recast[] = {0, 0, 0, 0};
        for (int i = 0; i < len; i += increment) {
            temp = 0;

            switch (_bit) {
                case 8:

                    recast[0] = (int8_t)buf[i + 0];
                    recast[1] = (int8_t)buf[i + 1];
                    recast[2] = (int8_t)buf[i + 2];
                    recast[3] = (int8_t)buf[i + 3];
                    break;
                case 16:
                    recast[0] = (((int16_t)buf[i + 0]) >> 0) & 0xFF;
                    recast[1] = (((int16_t)buf[i + 0]) >> 8) & 0xFF;
                    recast[2] = (((int16_t)buf[i + 1]) >> 0) & 0xFF;
                    recast[3] = (((int16_t)buf[i + 1]) >> 8) & 0xFF;
                    break;
                case 32:
                    recast[0] = (((int32_t)buf[i + 0]) >> 0) & 0xFF;
                    recast[1] = (((int32_t)buf[i + 0]) >> 8) & 0xFF;
                    recast[2] = (((int32_t)buf[i + 0]) >> 16) & 0xFF;
                    recast[3] = (((int32_t)buf[i + 0]) >> 24) & 0xFF;
                    break;
            }
            for (int j = 0; j < 4; j++) {
                temp |= recast[j] << (j * 8);
            }

            I2S0->TDR[0] = temp;
        }
    }
}

void FrdmI2s::write(int bufr[], int bufl[], int len) {
    //#TODO: Write this!
}

int FrdmI2s::read() { return I2S0->RDR[0]; }

void FrdmI2s::read(char buf[], int len) {
    bool len_valid = true;
    if (len <= 0) return;
    if (len >= fifo_points()) len = fifo_points();
    int temp[8];
    int counter = 0;
    int increment = 4;  // 32/_bit;
    int fifo_levl = fifo_level();
    while (counter < fifo_levl && len_valid) {
        temp[counter] = I2S0->RDR[0];
        for (int j = 0; j < increment; j++) {
            if ((counter * 4) + j > len) {
                len_valid = false;
                break;
            }
            buf[counter + j] = temp[counter] >> (j * 8);
        }
        counter++;
    }
}

void FrdmI2s::read(int buf[], int len) {
    bool len_valid = true;
    if (len <= 0) return;
    if (len >= fifo_points()) len = fifo_points();
    int temp[8];
    int counter = 0;
    int increment = 32 / _bit;
    int fifo_levl = fifo_level();
    while (counter < fifo_levl && len_valid) {
        temp[counter] = I2S0->RDR[0];
        for (int j = 0; j < increment; j++) {
            if ((counter * increment) + j > len) {
                len_valid = false;
                break;
            }
            buf[counter + j] = temp[counter] >> (j * _bit);
        }
        counter++;
    }
}

void FrdmI2s::read(int bufr[], int bufl[], int len) {
    //#TODO: Write this
}

int FrdmI2s::get_fifo_limit() {
    switch (_bit) {
        case 8:
            return (4 * 8);
        case 16:
            return (2 * 8);
        case 32:
            return 8;
        default:
            return 0;
    }
}

int FrdmI2s::fifo_points() {
    switch (_bit) {
        case 8:
            return (4 * fifo_level());
        case 16:
            return (2 * fifo_level());
        case 32:
            return fifo_level();
        default:
            return 0;
    }
}

void FrdmI2s::role(int mastermode) { _role = mastermode; }

void FrdmI2s::wordsize(int words) {
    _bit = 16;
    //    update_config();
}

void FrdmI2s::mclk_freq(int mclk) {
    mclk_frequency = 12288000;
    //    update_config();
}

void FrdmI2s::frequency(int wclk) {
    freq = 32000;
    _i2s_set_rate(freq);
    // update_config();
}

int FrdmI2s::fifo_level() {
    int tfr = 0;
    int wfp = 0;
    int rfp = 0;
    int level = 0;

    if (_rxtx == TRANSMIT) {
        tfr = I2S0->TFR[0];
        wfp = (tfr & I2S_TFR_WFP_MASK) >> I2S_TFR_WFP_SHIFT;
        rfp = (tfr & I2S_TFR_RFP_MASK) >> I2S_TFR_RFP_SHIFT;
    } else {
        tfr = I2S0->RFR[0];
        wfp = (tfr & I2S_RFR_WFP_MASK) >> I2S_RFR_WFP_SHIFT;
        rfp = (tfr & I2S_RFR_RFP_MASK) >> I2S_RFR_RFP_SHIFT;
    }
    level = 0x0F & (rfp - wfp);

    return level;
}

void FrdmI2s::stereomono(int stereomode) { _stereo = stereomode; }

void FrdmI2s::mute() { _mute = MUTED; }

void FrdmI2s::mute(int mute_en) { _mute = mute_en; }

void FrdmI2s::set_interrupt_fifo_level(int level) { interrupt_fifo_level = 4; }

void FrdmI2s::stop() {
    _stat = STOP;
    _mute = MUTED;
}

void FrdmI2s::start() {
    _stat = RUN;
    _mute = UNMUTED;
}

void FrdmI2s::power(bool pwr) {
    if (pwr) {
        _stat = STOP;
    } else {
        _stat = RUN;
    }
    update_config();
}

bool FrdmI2s::setup_ok() {
    if ((reg_write_err + pin_setup_err) > 0)
        return false;
    else
        return true;
}

void FrdmI2s::pin_setup() {
    pin_setup_err = 0;

    printf("\n\rSetting up pins....\n\r");
    if (_rxtx == TRANSMIT) {
        if (IoPin != PTC1) pin_setup_err++;
        if (WclkPin != PTE11) pin_setup_err++;
        if (BclkPin != PTE12) pin_setup_err++;
    } else {
        if (IoPin != PTE7) pin_setup_err++;
        if (WclkPin != PTE8) pin_setup_err++;
        if (BclkPin != PTC9) pin_setup_err++;
    }
    printf("Hmm....%i\n\r", pin_setup_err);
    /*
     * @param SerialData    The serial data pin
     * @param WordSelect    The word select pin
     * @param BitClk    The clock pin
     */

    SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTE_MASK;
    if (pin_setup_err == 0) {
        PORT_SetPinMux(PORTC, 6, kPORT_MuxAlt6);  // PTC6 I2S0_MCLK

        if (_rxtx == TRANSMIT) {
            PORT_SetPinMux(PORTC, 1, kPORT_MuxAlt6);  // PTC1 I2S0_TXD0

            PORT_SetPinMux(PORTE, 11, kPORT_MuxAlt4);  // PTE11 I2S0_TX_FS

            PORT_SetPinMux(PORTE, 12, kPORT_MuxAlt4);  // PTE12 I2S0_TX_BCLK

        } else {
            PORT_SetPinMux(PORTE, 7, kPORT_MuxAlt4);  // PTE7 I2S0_RXD0

            PORT_SetPinMux(PORTE, 8, kPORT_MuxAlt4);  // PTE8 I2S0_RX_FS

            PORT_SetPinMux(PORTC, 9, kPORT_MuxAlt4);  // PTC9 I2S0_RX_BCLK
        }
    }
}

void FrdmI2s::_set_clock_112896(void) {
    //    SIM->SCGC6 &= ~(SIM_SCGC6_I2S_MASK);

    // output = input[(I2SFRAC+1) / (I2SDIV+1) ] = (48* (4/17))
    // SIM_CLKDIV2 |= SIM_CLKDIV2_I2SDIV(16) | SIM_CLKDIV2_I2SFRAC(3);
    I2S0->MDR = I2S_MDR_FRACT(3) | I2S_MDR_DIVIDE(16);
    //    SIM->SCGC6 |= SIM_SCGC6_I2S_MASK;
}
void FrdmI2s::_set_clock_122800(void) {
    // output = input [(I2SFRAC+1) / (I2SDIV+1) ] = (120M* (64/625))
    // SIM_CLKDIV2 |= SIM_CLKDIV2_I2SDIV(625) | SIM_CLKDIV2_I2SFRAC(63);
    I2S0->MDR = I2S_MDR_FRACT(63) | I2S_MDR_DIVIDE(624);
}
void FrdmI2s::_i2s_init(void) {
/**
See note.md might give you more details
*/
#define I2S_CONFIG_WORDS_IN_A_FRAME 2
#define I2S_CONFIG_BITS_IN_A_WORD 16

    // enable TX
    I2S0->TCSR = I2S_TCSR_TE(1) |    // enable tx
                 I2S_TCSR_FRIE(1) |  // enable FIFO request interrupt
                 I2S_TCSR_BCE(1);    // enable bit clock

    I2S0->TCR1 = I2S_TCR1_TFW(4);    // 6;    // water mark
    I2S0->TCR2 = I2S_TCR2_SYNC(0) |  // master mode(Async mode)
                 I2S_TCR2_MSEL(1) |  // MSEL = internal
                                     // +------------------------+---------------+
                                     // | TCR2[MSEL], RCR2[MSEL] | Master Clock  |
                                     // +========================+===============+
                                     // | 00                     | Bus Clock     |
                                     // +------------------------+---------------+
                                     // | 01                     | I2S0_MCLK     |
                                     // +------------------------+---------------+
                                     // | 10                     | Not supported |
                                     // +------------------------+---------------+
                                     // | 11                     | Not supported |
                                     // +------------------------+---------------+
                 I2S_TCR2_BCP(1) |   // BCLK = drive on falling edge, sample on rising edge
                 I2S_TCR2_BCD(1);    // BCLK = OUTPUT

    I2S0->TCR3 = I2S_TCR3_TCE(1);  // enable channel 0

    I2S0->TCR4 = I2S_TCR4_FRSZ(I2S_CONFIG_WORDS_IN_A_FRAME - 1) |  // words in a frame
                 I2S_TCR4_SYWD(I2S_CONFIG_BITS_IN_A_WORD - 1) |    // bits in a word
                 I2S_TCR4_MF(1) |                                  // MSB first
                 I2S_TCR4_FSE(1) |                                 // one bit early
                 I2S_TCR4_FSP(1) |                                 // frame active low
                 I2S_TCR4_FSD(1);                                  // frame = output

    I2S0->TCR5 = I2S_TCR5_WNW((I2S_CONFIG_BITS_IN_A_WORD - 1)) |  // word N width
                 I2S_TCR5_W0W((I2S_CONFIG_BITS_IN_A_WORD - 1)) |  // word 0 width
                 I2S_TCR5_FBT(15);                                // right adjust, where the first bit starts

    I2S0->TMR = I2S_TMR_TWM(0);
    I2S0->MCR = I2S_MCR_MOE(1) |  // MCLK = output
                I2S_MCR_MICS(0);  // MCLK SRC = core clock = 48M
                                  // Select MCLK input source
                                  // +-----------+------------------------------------+
                                  // | MCR[MICS] | Clock Selection                    |
                                  // +===========+====================================+
                                  // | 00        | System clock                       |
                                  // +-----------+------------------------------------+
                                  // | 01        | OSC0ERCLK                          |
                                  // +-----------+------------------------------------+
                                  // | 10        | Not supported                      |
                                  // +-----------+------------------------------------+
                                  // | 11        | MCGPLLCK , IRC48MCLK, or MCGFLLCLK |
                                  // +-----------+------------------------------------+
    I2S0->MDR = I2S_MDR_FRACT(63) | I2S_MDR_DIVIDE(624);
}

void FrdmI2s::_i2s_set_rate(int smprate) {
    unsigned char div;
    //    SIM->SCGC6 |= SIM_SCGC6_I2S_MASK;
    if ((smprate % 441) == 0) {
        // 11k/ 22k/ 44k
        _set_clock_112896();
        mclk_frequency = 11289600;
    } else if ((smprate % 400) == 0) {
        // 8000/ 12000/ 16000/ 24000/ 32000/ 48000
        _set_clock_122800();
        mclk_frequency = 12288000;
    }

    switch (smprate) {
        case 32000:
            div = 5;
            break;  // 12.288M/(32K*16*2) = 12, 12 = (DIV+1)*2, DIV = 5
    }

    I2S0->TCR2 |= I2S_TCR2_DIV(div);
}

void FrdmI2s::update_config() {
    // reg_write_err = 0;
    // // Clock Multiplier Calculations
    // float pre_mult = 0;
    // int pre_num = 0;
    // int pre_den = 0;
    // int bitrate_div = 0;
    // // Clock Multiplier, MCLK setup
    // if (_rxtx == TRANSMIT) {
    //     int regvals = ((pre_num << 8) & 0xFF00) | (pre_den & 0xFF);
    // } else {
    // }
    //
    // switch (_bit) {
    //     case 8:
    //         wordwidth_code = 0;
    //         break;
    //     case 16:
    //         wordwidth_code = 1;
    //         break;
    //     case 32:
    //         wordwidth_code = 3;
    //         break;
    //     default:
    //         reg_write_err++;
    //         break;
    // }
    //
    // int I2SDA_reg = (wordwidth_code & 0x3);
    // I2SDA_reg |= ((!stereo << 2) & 0x4);
    // I2SDA_reg |= ((_stat << 3) & 0x8);
    // I2SDA_reg |= ((!master << 5) & 0x20);
    // I2SDA_reg |= (0x1F << 6);
    // I2SDA_reg |= ((muted << 15) & 0x8000);
    //
    // if (_rxtx == I2S_TRANSMIT) {
    //     if (txisr) {
    //         //
    //     } else {
    //         //
    //     }
    // } else {
    //     if (rxisr) {
    //         //
    //     } else {
    //         //
    //     }
    // }
}

void FrdmI2s::_i2sisr(void) {
    I2STXISR.call();
    I2SRXISR.call();
}

#endif  // defined(TARGET_K66F)
#endif  // 0
