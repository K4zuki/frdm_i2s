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
#if defined(TARGET_MCU_K64F)
// see https://developer.mbed.org/media/uploads/GregC/k64f_rm_rev2.pdf page 1721 for details
#include "FrdmI2s.h"

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

FrdmI2s::FrdmI2s(bool rxtx, PinName SerialData, PinName WordSelect, PinName BitClk) {
    SIM->SCGC6 &= ~(SIM_SCGC6_I2S_MASK);
    SIM->SCGC6 |= SIM_SCGC6_I2S(1);

    NVIC_DisableIRQ(I2S0_Tx_IRQn);
    NVIC_DisableIRQ(I2S0_Rx_IRQn);

    DataPin = SerialData;
    WclkPin = WordSelect;
    BclkPin = BitClk;
    _rxtx = rxtx;

    WordSelect_d = true;
    BitClk_d = true;
    MasterClk_d = false;

    fourwire = false;

    reg_write_err = 0;

    pin_setup();

    if (pin_setup_err != 0) {
        perror("I2S Pins incorrectly defined.");
    }

    defaulter();
    _i2s_init();
}

// FrdmI2s::I2S(bool rxtx, PinName SerialData) {
//     NVIC_DisableIRQ(I2S0_Tx_IRQn);
//     NVIC_DisableIRQ(I2S0_Rx_IRQn);
//
//     DataPin = SerialData;
//     _rxtx = rxtx;
//
//     WordSelect_d = false;
//     BitClk_d = false;
//     MasterClk_d = false;
//
//     fourwire = false;
//
//     reg_write_err = 0;
//
//     pin_setup();
//
//     if (pin_setup_err != 0) {
//         perror("I2S Pins incorrectly defined.");
//     }
//
//     defaulter();
// }

// FrdmI2s::I2S(bool rxtx, PinName SerialData, bool fourwiremode) {
//     pin_setup();
//
//     if (pin_setup_err != 0) {
//         perror("I2S Pins incorrectly defined.");
//     }
//
//     defaulter();
// }

// FrdmI2s::I2S(bool rxtx, PinName SerialData, PinName WordSelect, bool fourwiremode) {
//     NVIC_DisableIRQ(I2S0_Tx_IRQn);
//     NVIC_DisableIRQ(I2S0_Rx_IRQn);
//
//     DataPin = SerialData;
//     WclkPin = WordSelect;
//     _rxtx = rxtx;
//
//     WordSelect_d = true;
//     BitClk_d = false;
//     MasterClk_d = false;
//
//     reg_write_err = 0;
//
//     fourwire = fourwiremode;
//
//     pin_setup();
//
//     if (pin_setup_err != 0) {
//         perror("I2S Pins incorrectly defined.");
//     }
//
//     defaulter();
// }

// FrdmI2s::I2S(bool rxtx, PinName SerialData, PinName WordSelect) {
//     NVIC_DisableIRQ(I2S0_Tx_IRQn);
//     NVIC_DisableIRQ(I2S0_Rx_IRQn);
//
//     DataPin = SerialData;
//     WclkPin = WordSelect;
//     _rxtx = rxtx;
//
//     WordSelect_d = true;
//     BitClk_d = false;
//     MasterClk_d = false;
//
//     reg_write_err = 0;
//
//     fourwire = false;
//
//     pin_setup();
//
//     if (pin_setup_err != 0) {
//         perror("I2S Pins incorrectly defined.");
//     }
//
//     defaulter();
// }

FrdmI2s::~FrdmI2s() {
    NVIC_DisableIRQ(I2S0_Tx_IRQn);
    NVIC_DisableIRQ(I2S0_Rx_IRQn);

    deallocating = true;
    pin_setup();
    update_config();
}

void FrdmI2s::defaulter() {
    I2S0->TCSR |= I2S_TCSR_TE(1);

    stop();
    master = false;
    deallocating = false;

    frequency(defaults.SAMPLERATE);
    wordsize(defaults.WORDWIDTH);
    masterslave(defaults.MASTERSLAVE);
    stereomono(defaults.STEREOMONO);
    set_interrupt_fifo_level(defaults.INTERRUPT_FIFO_LEVEL);
    mute(defaults.MUTED);

    NVIC_SetVector(I2S0_Tx_IRQn, (uint32_t)&_i2sisr);
    NVIC_EnableIRQ(I2S0_Tx_IRQn);
}

void FrdmI2s::write(char buf[], int len) {
    if (_rxtx == I2S_TRANSMIT) {
        if (len > max_fifo_points()) {
            len = max_fifo_points();
        }
        if (len <= 0) {
            return;
        }
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
    if (_rxtx == I2S_TRANSMIT && wordwidth > 0) {
        if (len > max_fifo_points()) {
            len = max_fifo_points();
            printf("Trying to write too much data!\n\r");
        }
        if (len <= 0) {
            return;
        }
        uint32_t temp = 0;
        int increment = 32 / wordwidth;
        unsigned char recast[] = {0, 0, 0, 0};
        for (int i = 0; i < len; i += increment) {
            temp = 0;

            switch (wordwidth) {
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

            // if(((temp >> 16) & 0xFFFF) == 0xFFFF) printf("Hmmm %x %x %x\n\r",temp,
            // increment,i); //|| temp &0xFFFF == 0xFFFF
            // if((buf[i]-buf[i+1])>5000 || (buf[i]-buf[i+1])<-5000)
            // printf("J:%i,%i\n\r",buf[i],buf[i+1]);
            // printf("%x\n",temp);
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
    if (len <= 0) {
        return;
    }
    if (len >= fifo_points()) {
        len = fifo_points();
    }
    int temp[8];
    int counter = 0;
    int increment = 4;  // 32/wordwidth;
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
    if (len <= 0) {
        return;
    }
    if (len >= fifo_points()) {
        len = fifo_points();
    }
    int temp[8];
    int counter = 0;
    int increment = 32 / wordwidth;
    int fifo_levl = fifo_level();
    while (counter < fifo_levl && len_valid) {
        temp[counter] = I2S0->RDR[0];
        for (int j = 0; j < increment; j++) {
            if ((counter * increment) + j > len) {
                len_valid = false;
                break;
            }
            buf[counter + j] = temp[counter] >> (j * wordwidth);
        }
        counter++;
    }
}

void FrdmI2s::read(int bufr[], int bufl[], int len) {
    //#TODO: Write this
}

int FrdmI2s::max_fifo_points() {
    switch (wordwidth) {
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
    switch (wordwidth) {
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

void FrdmI2s::power(bool pwr) {
    if (pwr) {
        stopped = false;
    } else {
        stopped = true;
    }
    update_config();
}

void FrdmI2s::masterslave(bool mastermode) {
    if (mastermode == I2S_MASTER) {
        master = true;
    } else {
        master = false;
    }
    update_config();
}

void FrdmI2s::wordsize(int words) {
    wordwidth = 16;
    //    update_config();
}

void FrdmI2s::mclk_freq(int freq) {
    mclk_frequency = 12288000;
    //    update_config();
}

void FrdmI2s::frequency(int desired_freq) {
    freq = 32000;
    _i2s_set_rate(freq);
    // update_config();
}

int FrdmI2s::fifo_level() {
    int level = 0;
    if (_rxtx == I2S_TRANSMIT) {
        level = I2S0->TFR[0];
        level >>= 16;
        level &= 0x0F;
    } else {
        level = I2S0->TFR[0];
        level >>= 0;
        level &= 0x0F;
    }
    return level;
}

void FrdmI2s::stereomono(bool stereomode) {
    stereo = true;
    /*
        if (stereomode == I2S_STEREO) {
            stereo = true;
        } else {
            stereo = false;
        }
    */
}

void FrdmI2s::mute() {
    muted = true;
    //    update_config();
}

void FrdmI2s::mute(bool mute_en) {
    muted = mute_en;
    //    update_config();
}

void FrdmI2s::stop() {
    stopped = true;
    //    update_config();
}

void FrdmI2s::set_interrupt_fifo_level(int level) {
    interrupt_fifo_level = 4;
    //    update_config();
}

void FrdmI2s::start() {
    stopped = false;
    muted = false;
    //    update_config();
}

bool FrdmI2s::setup_ok() {
    if ((reg_write_err + pin_setup_err) > 0)
        return false;
    else
        return true;
}

void FrdmI2s::pin_setup() {
    pin_setup_err = 0;

    if (_rxtx == I2S_TRANSMIT) {
        printf("\n\rSetting up pins....\n\r");
        if (DataPin != PTC1) pin_setup_err++;
        if (WclkPin != PTB19 && WordSelect_d == true) pin_setup_err++;
        if (BclkPin != PTB18 && BitClk_d == true) pin_setup_err++;
        printf("Hmm....%i\n\r", pin_setup_err);
    } else {
        if (DataPin != PTC5) pin_setup_err++;
        if (WclkPin != PTC7 && WordSelect_d == true) pin_setup_err++;
        if (BclkPin != PTC6 && BitClk_d == true) pin_setup_err++;
    }
    /*
     * @param SerialData    The serial data pin
     * @param WordSelect    The word select pin
     * @param BitClk    The clock pin
    PORTC->PCR[8]  &= PORT_PCR_MUX_MASK;
    PORTC->PCR[8]  |= PORT_PCR_MUX(0x04); // PTC8 I2S0_MCLK

    PORTC->PCR[5]  &= PORT_PCR_MUX_MASK;
    PORTC->PCR[5]  |= PORT_PCR_MUX(0x04); // PTC5 I2S0_RXD0

    PORTC->PCR[7] &= PORT_PCR_MUX_MASK;
    PORTC->PCR[7] |= PORT_PCR_MUX(0x04); // PTC7 I2S0_RX_FS

    PORTC->PCR[6] &= PORT_PCR_MUX_MASK;
    PORTC->PCR[6] |= PORT_PCR_MUX(0x04); // PTC6 I2S0_RX_BCLK

    PORTC->PCR[1] &= PORT_PCR_MUX_MASK;
    PORTC->PCR[1] |= PORT_PCR_MUX(0x04); // PTC1 I2S0_TXD0

    PORTB->PCR[19] &= PORT_PCR_MUX_MASK;
    PORTB->PCR[19] |= PORT_PCR_MUX(0x04); // PTB19 I2S0_TX_FS

    PORTB->PCR[18] &= PORT_PCR_MUX_MASK;
    PORTB->PCR[18] |= PORT_PCR_MUX(0x04); // PTB18 I2S0_TX_BCLK
     */

    SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTC_MASK;
    if (pin_setup_err == 0) {
        PORTC->PCR[8] &= PORT_PCR_MUX_MASK;
        PORTC->PCR[8] |= PORT_PCR_MUX(0x04);  // PTC8 I2S0_MCLK

        if (_rxtx == I2S_TRANSMIT) {
            int val1 = 1;
            if (deallocating) {
                val1 = 0;
            }

            PORTC->PCR[1] &= PORT_PCR_MUX_MASK;
            PORTC->PCR[1] |= PORT_PCR_MUX(0x04);  // PTC1 I2S0_TXD0

            if (WordSelect_d == true) {
                PORTB->PCR[18] &= PORT_PCR_MUX_MASK;
                PORTB->PCR[18] |= PORT_PCR_MUX(0x04);  // PTB18 I2S0_TX_BCLK
            }
            if (BitClk_d == true) {
                PORTB->PCR[19] &= PORT_PCR_MUX_MASK;
                PORTB->PCR[19] |= PORT_PCR_MUX(0x04);  // PTB19 I2S0_TX_FS
            }

        } else {
            int val1 = 1;
            int val2 = 2;
            if (deallocating) {
                val1 = 0;
                val2 = 0;
            }

            PORTC->PCR[5] &= PORT_PCR_MUX_MASK;
            PORTC->PCR[5] |= PORT_PCR_MUX(0x04);  // PTC5 I2S0_RXD0

            if (WordSelect_d == true) {
                PORTB->PCR[18] &= PORT_PCR_MUX_MASK;
                PORTB->PCR[18] |= PORT_PCR_MUX(0x04);  // PTB18 I2S0_TX_BCLK
            }

            if (BitClk_d == true) {
                PORTC->PCR[6] &= PORT_PCR_MUX_MASK;
                PORTC->PCR[6] |= PORT_PCR_MUX(0x04);  // PTC6 I2S0_RX_BCLK
            }
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
    // output = input [(I2SFRAC+1) / (I2SDIV+1) ] = (48M* (32/125))
    // SIM_CLKDIV2 |= SIM_CLKDIV2_I2SDIV(124) | SIM_CLKDIV2_I2SFRAC(31);
    I2S0->MDR = I2S_MDR_FRACT(63) | I2S_MDR_DIVIDE(624);
}
void FrdmI2s::_i2s_init(void) {
#define I2S_CONFIG_WORDS_IN_A_FRAME 2
#define I2S_CONFIG_BITS_IN_A_WORD 16

    I2S0->TCR1 = I2S_TCR1_TFW(4);    // 6;    // water mark
    I2S0->TCR2 = I2S_TCR2_SYNC(0) |  // master mode(Async mode)
                 I2S_TCR2_MSEL(1) |  // MSEL = MCLK
                 I2S_TCR2_BCP(1) |   // CLK = drive on falling edge
                 I2S_TCR2_BCD(1);    // CLK = OUTPUT

    I2S0->TCR3 = I2S_TCR3_TCE(1);  // enable channel 0

    I2S0->TCR4 = I2S_TCR4_FRSZ(I2S_CONFIG_WORDS_IN_A_FRAME - 1) |  // words in a frame
                 I2S_TCR4_SYWD(I2S_CONFIG_BITS_IN_A_WORD - 1) |    // bits in a word
                 I2S_TCR4_MF(1) |                                  // MSB first
                 I2S_TCR4_FSE(1) |                                 // one bit early
                 I2S_TCR4_FSP(1) |                                 // frame active low
                 I2S_TCR4_FSD(0);                                  // frame = output

    I2S0->TCR5 = I2S_TCR5_WNW((I2S_CONFIG_BITS_IN_A_WORD - 1)) |  // word N width
                 I2S_TCR5_W0W((I2S_CONFIG_BITS_IN_A_WORD - 1)) |  // word 0 width
                 I2S_TCR5_FBT(23);                                // right adjust, where the first bit starts

    I2S0->TMR = I2S_TMR_TWM(0);

    // enable TX
    I2S0->TCSR = I2S_TCSR_TE(1) |  // enable tx
                 I2S_TCSR_BCE(1);  // enable bit clock
}

void FrdmI2s::_i2s_set_rate(int smprate) {
    unsigned char div;
    //    SIM->SCGC6 |= SIM_SCGC6_I2S_MASK;

    // Select MCLK input source
    I2S0->MCR = I2S_MCR_MOE(1) |  // MCLK = output
                I2S_MCR_MICS(0);  // MCLK SRC = core clock = 48M

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
            div = 3;
            break;  // 12.288M/(32K*48) = 8, 8 = (DIV+1)*2, DIV = 3
    }

    I2S0->TCR2 |= I2S_TCR2_DIV(3);
}

void FrdmI2s::update_config() {
    reg_write_err = 0;
    // Clock Multiplier Calculations
    float pre_mult = 0;
    int pre_num = 0;
    int pre_den = 0;
    int bitrate_div = 0;
    // if (master == true) {  // In the hope of not doing all this heavy lifting
    //     every configuration
    //         // printf("Doing some clock magic..\n\r");
    //         int bitrate = freq * 64;
    //     float target_div = I2S_PCLK_RATE / float(bitrate * 2);  // Work out
    //     what divider is needed in the end, including the halving of rates the smoother does if (mclk_frequency == 0)
    //     {
    //         float rnd = fmod(target_div, 1);  // To make the X/Y fraction
    //         closest to 1,
    //             we set the last divider to the nearest integer to the rate divider bitrate_div = int(target_div -
    //             rnd);
    //         while (bitrate_div > I2S_MAX_BITRATE_DIV) {  // But this might be
    //             out of range, so we right shift it into focus bitrate_div >>= 1;
    //         }
    //         if (bitrate_div == 0) {  // Could be zero, which would disable
    //             the the clock... bitrate_div = 1;
    //         }
    //         pre_mult = float(bitrate_div) / target_div;  // Work out what
    //         we have left to correct pre_num = 0;
    //         pre_den = 0;
    //         fraction_estimator(pre_mult, &pre_num, &pre_den);  // Get the
    //         function to work out the closest fraction,
    //             there might be some point in adding some possible multipliers of these values to add to the
    //             smoothing,
    //             the reference manual(UM10360 page 480) suggests this
    //     }
    //     else {
    //         pre_mult = float(mclk_frequency * 2) / (I2S_PCLK_RATE);
    //         pre_num = 0;
    //         pre_den = 0;
    //         fraction_estimator(pre_mult, &pre_num, &pre_den);  // Get the
    //         function to work out the closest fraction,
    //             there might be some point in adding some possible multipliers of these values to add to the
    //             smoothing,
    //             the reference manual(UM10360 page 480) suggests this bitrate_div =
    //                 int(I2S_PCLK_RATE * float(pre_num) / float(pre_den) / float(bitrate));
    //     }
    //
    //     old_freq = freq;
    //     old_pre_num = pre_num;
    //     old_pre_den = pre_den;
    //     old_bitrate_div = bitrate_div;
    // } else {
    //     pre_num = old_pre_num;
    //     pre_den = old_pre_den;
    //     bitrate_div = old_bitrate_div;
    // }

    // Clock Multiplier, MCLK setup
    if (_rxtx == I2S_TRANSMIT) {
        int regvals = ((pre_num << 8) & 0xFF00) | (pre_den & 0xFF);
        // LPC_I2S->I2STXRATE = regvals;                      // Setting the X/Y fraction
        // LPC_I2S->I2STXBITRATE = (bitrate_div - 1) & 0x3F;  // Setting up the
        //                                                    // bitrate divider, the periferal adds one to this
        //
        // LPC_I2S->I2STXMODE = fourwire << 2;

        if (MasterClk_d == true) {
            // LPC_I2S->I2STXMODE |= (1 << 3);
        }
    } else {
        // int regvals = ((pre_num << 8) & 0xFF00) | (pre_den & 0xFF);
        // LPC_I2S->I2SRXRATE = regvals;                      // Setting the X/Y fraction
        // LPC_I2S->I2SRXBITRATE = (bitrate_div - 1) & 0x3F;  // Setting up the
        // bitrate divider, the periferal adds one to this

        // LPC_I2S->I2SRXMODE = fourwire << 2;

        if (MasterClk_d == true) {
            // LPC_I2S->I2SRXMODE |= (1 << 3);
        }
    }

    switch (wordwidth) {
        case 8:
            wordwidth_code = 0;
            break;
        case 16:
            wordwidth_code = 1;
            break;
        case 32:
            wordwidth_code = 3;
            break;
        default:
            reg_write_err++;
            break;
    }

    int I2SDA_reg = (wordwidth_code & 0x3);
    I2SDA_reg |= ((!stereo << 2) & 0x4);
    I2SDA_reg |= ((stopped << 3) & 0x8);
    I2SDA_reg |= ((!master << 5) & 0x20);
    I2SDA_reg |= (0x1F << 6);
    I2SDA_reg |= ((muted << 15) & 0x8000);

    if (_rxtx == I2S_TRANSMIT) {
        ;
        // LPC_I2S->I2SDAO = I2SDA_reg;
    } else {
        ;
        // LPC_I2S->I2SDAI = I2SDA_reg;
    }

    if (_rxtx == I2S_TRANSMIT) {
        if (txisr) {
            // LPC_I2S->I2SIRQ = (LPC_I2S->I2SIRQ & 0xFF0FFFFF) | ((interrupt_fifo_level & 0xF) << 16);
            // LPC_I2S->I2SIRQ |= 0x2;
        } else {
            ;
            // LPC_I2S->I2SIRQ &= 0xFFFFFFFD;
        }
    } else {
        if (rxisr) {
            // LPC_I2S->I2SIRQ = (LPC_I2S->I2SIRQ & 0xFFFFF0FF) | ((interrupt_fifo_level & 0xF) << 8);
            // LPC_I2S->I2SIRQ |= 0x1;
        }

        else {
            ;
            // LPC_I2S->I2SIRQ &= 0xFFFFFFFE;
        }
    }
}

void FrdmI2s::_i2sisr(void) {
    I2STXISR.call();
    I2SRXISR.call();
}

// A function to find the nearest fraction to that put to it, with numerator and
// denomnator less than 256
// This is used when trying to get the clocks correct

// void FrdmI2s::fraction_estimator(float in, int* num, int* den) {
//     int test_num = 0;
//     int test_den = 0;
//     float least_error = 1;
//     int least_err_den = 0;
//     float genval;
//     float generr;
//
//     for (test_den = 1; test_den < I2S_MAX_DENOMINATOR; test_den++) {
//         test_num = int(float(test_den) * in);
//         if (test_num < I2S_MAX_NUMERATOR && test_num > 0) {
//             genval = float(test_num) / float(test_den);
//             generr = mod(genval - in);
//             if (generr < least_error) {
//                 least_error = generr;
//                 least_err_den = test_den;
//             }
//             if (generr == 0) {
//                 break;
//             }
//         }
//     }
//
//     test_num = int(float(least_err_den) * in);
//     *num = test_num;
//     *den = least_err_den;
// }

// float FrdmI2s::mod(float in) {
//     if (in < 0) in *= -1;
//
//     return in;
// }

#endif
