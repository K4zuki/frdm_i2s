#include "I2S.h"

#define I2S_DF_WORDWIDTH                16
#define I2S_DF_SAMPLERATE               32000
#define I2S_DF_MASTERSLAVE              I2S_MASTER
#define I2S_DF_STEREOMONO               I2S_STEREO
#define I2S_DF_MUTED                    I2S_UNMUTED
#define I2S_DF_INTERRUPT_FIFO_LEVEL     4

#define I2S_MAX_DENOMINATOR             256
#define I2S_MAX_NUMERATOR               256
#define I2S_MAX_BITRATE_DIV             64

#define I2S_PCLK_RATE                   12288000

FunctionPointer I2S::I2STXISR;
FunctionPointer I2S::I2SRXISR;

bool I2S::txisr;
bool I2S::rxisr;

I2S::I2S(bool rxtx, PinName SerialData, PinName WordSelect, PinName BitClk)
{
    NVIC_DisableIRQ (I2S0_Tx_IRQn);
    NVIC_DisableIRQ (I2S0_Rx_IRQn);

    _SerialData = SerialData;
    _WordSelect = WordSelect;
    _BitClk = BitClk;
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

//I2S::I2S(bool rxtx, PinName SerialData)
//{
//    NVIC_DisableIRQ (I2S0_Tx_IRQn);
//    NVIC_DisableIRQ (I2S0_Rx_IRQn);
//
//    _SerialData = SerialData;
//    _rxtx = rxtx;
//
//    WordSelect_d = false;
//    BitClk_d = false;
//    MasterClk_d = false;
//
//    fourwire = false;
//
//    reg_write_err = 0;
//
//    pin_setup();
//
//    if (pin_setup_err != 0) {
//        perror("I2S Pins incorrectly defined.");
//    }
//
//    defaulter();
//}
//
//I2S::I2S(bool rxtx, PinName SerialData, bool fourwiremode)
//{
//    NVIC_DisableIRQ (I2S0_Tx_IRQn);
//    NVIC_DisableIRQ (I2S0_Rx_IRQn);
//
//    _SerialData = SerialData;
//    _rxtx = rxtx;
//
//    WordSelect_d = false;
//    BitClk_d = false;
//    MasterClk_d = false;
//
//    reg_write_err = 0;
//
//    fourwire = fourwiremode;
//
//    pin_setup();
//
//    if (pin_setup_err != 0) {
//        perror("I2S Pins incorrectly defined.");
//    }
//
//    defaulter();
//}
//
//I2S::I2S(bool rxtx, PinName SerialData, PinName WordSelect, bool fourwiremode)
//{
//    NVIC_DisableIRQ (I2S0_Tx_IRQn);
//    NVIC_DisableIRQ (I2S0_Rx_IRQn);
//
//    _SerialData = SerialData;
//    _WordSelect = WordSelect;
//    _rxtx = rxtx;
//
//    WordSelect_d = true;
//    BitClk_d = false;
//    MasterClk_d = false;
//
//    reg_write_err = 0;
//
//    fourwire = fourwiremode;
//
//    pin_setup();
//
//    if (pin_setup_err != 0) {
//        perror("I2S Pins incorrectly defined.");
//    }
//
//    defaulter();
//}
//
//I2S::I2S(bool rxtx, PinName SerialData, PinName WordSelect)
//{
//    NVIC_DisableIRQ (I2S0_Tx_IRQn);
//    NVIC_DisableIRQ (I2S0_Rx_IRQn);
//
//    _SerialData = SerialData;
//    _WordSelect = WordSelect;
//    _rxtx = rxtx;
//
//    WordSelect_d = true;
//    BitClk_d = false;
//    MasterClk_d = false;
//
//    reg_write_err = 0;
//
//    fourwire = false;
//
//    pin_setup();
//
//    if (pin_setup_err != 0) {
//        perror("I2S Pins incorrectly defined.");
//    }
//
//    defaulter();
//}

I2S::~I2S()
{
    NVIC_DisableIRQ (I2S0_Tx_IRQn);
    NVIC_DisableIRQ (I2S0_Rx_IRQn);

    deallocating = true;
    pin_setup();
    write_registers();
}

void I2S::defaulter()
{
    I2S0_TCSR |= 1u<<31;

    stop();
    master = false;
    deallocating = false;

    frequency(I2S_DF_SAMPLERATE);
    wordsize(I2S_DF_WORDWIDTH);
    masterslave (I2S_DF_MASTERSLAVE);
    stereomono (I2S_DF_STEREOMONO);
    set_interrupt_fifo_level(I2S_DF_INTERRUPT_FIFO_LEVEL);
    mute (I2S_DF_MUTED);

    NVIC_SetVector(I2S0_Tx_IRQn, (uint32_t) & _i2sisr);
    NVIC_EnableIRQ (I2S0_Tx_IRQn);
}

void I2S::write(char buf[], int len)
{
    if (_rxtx == I2S_TRANSMIT) {
        if (len > max_fifo_points())
            len = max_fifo_points();
        if (len <= 0)
            return;
        int temp = 0;
        for (int i = 0; i < len; i += 4) {
            temp = 0;
            for (int j = 0; j < 4; j++) {
                temp |= int(buf[i + j]) << (j * 8);
            }
            I2S0_TDR0 = temp;
        }
    }

}

void I2S::write(int buf[], int len)
{
    if (_rxtx == I2S_TRANSMIT && wordwidth > 0) {
        if (len > max_fifo_points()) {
            len = max_fifo_points();
            printf("Trying to write too much data!\n\r");
        }
        if (len <= 0)
            return;
        uint32_t temp = 0;
        int increment = 32 / wordwidth;
        unsigned char recast[] =
        { 0, 0, 0, 0 };
        for (int i = 0; i < len; i += increment) {
            temp = 0;

            switch (wordwidth) {

                case 8:

                    recast[0] = (int8_t) buf[i + 0];
                    recast[1] = (int8_t) buf[i + 1];
                    recast[2] = (int8_t) buf[i + 2];
                    recast[3] = (int8_t) buf[i + 3];
                    break;
                case 16:
                    recast[0] = (((int16_t) buf[i + 0]) >> 0) & 0xFF;
                    recast[1] = (((int16_t) buf[i + 0]) >> 8) & 0xFF;
                    recast[2] = (((int16_t) buf[i + 1]) >> 0) & 0xFF;
                    recast[3] = (((int16_t) buf[i + 1]) >> 8) & 0xFF;
                    break;
                case 32:
                    recast[0] = (((int32_t) buf[i + 0]) >> 0) & 0xFF;
                    recast[1] = (((int32_t) buf[i + 0]) >> 8) & 0xFF;
                    recast[2] = (((int32_t) buf[i + 0]) >> 16) & 0xFF;
                    recast[3] = (((int32_t) buf[i + 0]) >> 24) & 0xFF;
                    break;

            }
            for (int j = 0; j < 4; j++) {

                temp |= recast[j] << (j * 8);
            }

            //if(((temp >> 16) & 0xFFFF) == 0xFFFF) printf("Hmmm %x %x %x\n\r",temp, increment,i); //|| temp &0xFFFF == 0xFFFF
            //if((buf[i]-buf[i+1])>5000 || (buf[i]-buf[i+1])<-5000) printf("J:%i,%i\n\r",buf[i],buf[i+1]);
            //printf("%x\n",temp);
            I2S0_TDR0 = temp;
        }
    }
}

void I2S::write(int bufr[], int bufl[], int len)
{
    //#TODO: Write this!
}

int I2S::read()
{
    return I2S0_RDR0;
}

void I2S::read(char buf[], int len)
{
    bool len_valid = true;
    if (len <= 0)
        return;
    if (len >= fifo_points())
        len = fifo_points();
    int temp[8];
    int counter = 0;
    int increment = 4;            //32/wordwidth;
    int fifo_levl = fifo_level();
    while (counter < fifo_levl && len_valid) {
        temp[counter] = I2S0_RDR0;
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

void I2S::read(int buf[], int len)
{
    bool len_valid = true;
    if (len <= 0)
        return;
    if (len >= fifo_points())
        len = fifo_points();
    int temp[8];
    int counter = 0;
    int increment = 32 / wordwidth;
    int fifo_levl = fifo_level();
    while (counter < fifo_levl && len_valid) {
        temp[counter] = I2S0_RDR0;
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

void I2S::read(int bufr[], int bufl[], int len)
{
    //#TODO: Write this
}

int I2S::max_fifo_points()
{
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

int I2S::fifo_points()
{
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

void I2S::power(bool pwr)
{
    if (pwr) {
        stopped = false;
    } else {
        stopped = true;
    }
    write_registers();
}

void I2S::masterslave(bool mastermode)
{
    if (mastermode == I2S_MASTER) {
        master = true;
    } else {
        master = false;
    }
    write_registers();
}

void I2S::wordsize(int words)
{
    wordwidth = 16;
//    write_registers();
}

void I2S::mclk_freq(int freq)
{
    mclk_frequency = 12288000;
    write_registers();
}

void I2S::frequency(int desired_freq)
{
    freq = 32000;
    _i2s_set_rate(freq);
    //write_registers();
}

int I2S::fifo_level()
{
    int level = 0;
    if (_rxtx == I2S_TRANSMIT) {
        level = I2S0_TFR0;
        level >>= 16;
        level &= 0xF;
    } else {
        level = I2S0_TFR0;
        level >>= 0;
        level &= 0xF;
    }
    return level;
}

void I2S::stereomono(bool stereomode)
{
    stereo = true;
    /*
        if (stereomode == I2S_STEREO) {
            stereo = true;
        } else {
            stereo = false;
        }
    */
}

void I2S::mute()
{
    muted = true;
//    write_registers();
}

void I2S::mute(bool mute_en)
{
    muted = mute_en;
//    write_registers();
}

void I2S::stop()
{
    stopped = true;
//    write_registers();
}

void I2S::set_interrupt_fifo_level(int level)
{
    interrupt_fifo_level = 4;
//    write_registers();
}

void I2S::start()
{
    stopped = false;
    muted = false;
//    write_registers();
}

bool I2S::setup_ok()
{
    if ((reg_write_err + pin_setup_err) > 0)
        return false;
    else
        return true;
}

void I2S::pin_setup()
{
    pin_setup_err = 0;

    if (_rxtx == I2S_TRANSMIT) {
        printf("\n\rSetting up pins....\n\r");
        if (_SerialData != PTC1)
            pin_setup_err++;
        if (_WordSelect != PTB19 && WordSelect_d == true)
            pin_setup_err++;
        if (_BitClk != PTB18 && BitClk_d == true)
            pin_setup_err++;
        printf("Hmm....%i\n\r", pin_setup_err);
    } else {
        if (_SerialData != PTC5)
            pin_setup_err++;
        if (_WordSelect != PTC7 && WordSelect_d == true)
            pin_setup_err++;
        if (_BitClk != PTC6 && BitClk_d == true)
            pin_setup_err++;
    }
    /*
     * @param SerialData    The serial data pin
     * @param WordSelect    The word select pin
     * @param BitClk    The clock pin
    PORTC_PCR8  &= PORT_PCR_MUX_MASK;
    PORTC_PCR8  |= PORT_PCR_MUX(0x04); // PTC8 I2S0_MCLK

    PORTC_PCR5  &= PORT_PCR_MUX_MASK;
    PORTC_PCR5  |= PORT_PCR_MUX(0x04); // PTC5 I2S0_RXD0

    PORTC_PCR7 &= PORT_PCR_MUX_MASK;
    PORTC_PCR7 |= PORT_PCR_MUX(0x04); // PTC7 I2S0_RX_FS

    PORTC_PCR6 &= PORT_PCR_MUX_MASK;
    PORTC_PCR6 |= PORT_PCR_MUX(0x04); // PTC6 I2S0_RX_BCLK

    PORTC_PCR1 &= PORT_PCR_MUX_MASK;
    PORTC_PCR1 |= PORT_PCR_MUX(0x04); // PTC1 I2S0_TXD0

    PORTB_PCR19 &= PORT_PCR_MUX_MASK;
    PORTB_PCR19 |= PORT_PCR_MUX(0x04); // PTB19 I2S0_TX_FS

    PORTB_PCR18 &= PORT_PCR_MUX_MASK;
    PORTB_PCR18 |= PORT_PCR_MUX(0x04); // PTB18 I2S0_TX_BCLK
     */


    if (pin_setup_err == 0) {
        if (_rxtx == I2S_TRANSMIT) {
            int val1 = 1;
            if (deallocating) {
                val1 = 0;
            }
            PORTC_PCR1 &= PORT_PCR_MUX_MASK;
            PORTC_PCR1 |= PORT_PCR_MUX(0x04); // PTC1 I2S0_TXD0
            if (WordSelect_d == true) {
                PORTB_PCR18 &= PORT_PCR_MUX_MASK;
                PORTB_PCR18 |= PORT_PCR_MUX(0x04); // PTB18 I2S0_TX_BCLK
            }
            if (BitClk_d == true) {
                PORTB_PCR19 &= PORT_PCR_MUX_MASK;
                PORTB_PCR19 |= PORT_PCR_MUX(0x04); // PTB19 I2S0_TX_FS
            }

        } else {
            int val1 = 1;
            int val2 = 2;
            if (deallocating) {
                val1 = 0;
                val2 = 0;
            }

            PORTC_PCR5  &= PORT_PCR_MUX_MASK;
            PORTC_PCR5  |= PORT_PCR_MUX(0x04); // PTC5 I2S0_RXD0

            if (WordSelect_d == true) {
                PORTB_PCR18 &= PORT_PCR_MUX_MASK;
                PORTB_PCR18 |= PORT_PCR_MUX(0x04); // PTB18 I2S0_TX_BCLK
            }

            if (BitClk_d == true) {
                PORTC_PCR6 &= PORT_PCR_MUX_MASK;
                PORTC_PCR6 |= PORT_PCR_MUX(0x04); // PTC6 I2S0_RX_BCLK
            }
        }
    }
}







void I2S::_set_clock_112896(void)
{
    SIM_SCGC6 &= ~(SIM_SCGC6_I2S_MASK);

    // output = input[(I2SFRAC+1) / (I2SDIV+1) ] = (48* (4/17))
    // SIM_CLKDIV2 |= SIM_CLKDIV2_I2SDIV(16) | SIM_CLKDIV2_I2SFRAC(3);
    I2S0_MDR = I2S_MDR_FRACT(3) | I2S_MDR_DIVIDE(16);
    SIM_SCGC6 |= SIM_SCGC6_I2S_MASK;
}
void I2S::_set_clock_122800(void)
{
    // output = input [(I2SFRAC+1) / (I2SDIV+1) ] = (48M* (32/125))
    // SIM_CLKDIV2 |= SIM_CLKDIV2_I2SDIV(124) | SIM_CLKDIV2_I2SFRAC(31);
    I2S0_MDR = I2S_MDR_FRACT(31) | I2S_MDR_DIVIDE(124);
}
void I2S::_i2s_init(void)
{
#define I2S_CONFIG_WORDS_IN_A_FRAME 2
#define I2S_CONFIG_BITS_IN_A_WORD   16

    I2S0_TCR1 = 4;// 6;    // water mark
    I2S0_TCR2 |= (0<<30) | // master mode(Async mode)
                 (1<<26) | // MSEL = MCLK
                 (1<<25) | // CLK = drive on falling edge
                 (1<<24) ; // CLK = OUTPUT

    I2S0_TCR3 = (1<<16); // enable channel 0

    I2S0_TCR4 = ((I2S_CONFIG_WORDS_IN_A_FRAME-1)<<16)  | // words in a frame
                ((I2S_CONFIG_BITS_IN_A_WORD  -1)<<8)   | // bits in a word
                (1<<4)                                | // MSB
                (1<<3)                                | // one bit early
                (1<<1)                                | // frame active low
                (1<<0)                                ; // frame = output

    I2S0_TCR5 = ((I2S_CONFIG_BITS_IN_A_WORD-1) <<24) | // word N width
                ((I2S_CONFIG_BITS_IN_A_WORD-1) <<16) | // word 0 width
                (0x17<<8);                            // right adjust, where the first bit starts

    I2S0_TMR = 0;

    // enable TX
    I2S0_TCSR = (0<<31) | // enable tx
                (1<<28) | // enable bit clock
                (0<<0);   // enable DMA request
}

void I2S::_i2s_set_rate(int smprate)
{
    unsigned char div;
    SIM_SCGC6 |= SIM_SCGC6_I2S_MASK;

    // Select MCLK input source
    I2S0_MCR = (1<<30)| // MCLK = output
               (0<<24); // MCLK SRC = core clock = 48M

    if((smprate == 11025)||(smprate == 22050)||(smprate == 44100)) {
        _set_clock_112896();
        mclk_frequency = 11289600;
    }

    if((smprate == 8000) || (smprate == 12000) || (smprate == 16000) ||
            (smprate == 24000)|| (smprate == 32000) || (smprate == 48000) ) {
        _set_clock_122800();
        mclk_frequency = 12288000;
    }

    switch(smprate) {
        case 32000:
            div=3;
            break; // 12.288M/(32K*48) = 8, 8 = (DIV+1)*2, DIV = 3
    }

    I2S0_TCR2 = div;
}










void I2S::write_registers()
{
    reg_write_err = 0;
    //Clock Multiplier Calculations
    float pre_mult = 0;
    int pre_num = 0;
    int pre_den = 0;
    int bitrate_div = 0;
//    if (master == true) { // In the hope of not doing all this heavy lifting every configuration
//        //printf("Doing some clock magic..\n\r");
//        int bitrate = freq * 64;
//        float target_div = I2S_PCLK_RATE / float(bitrate * 2);// Work out what divider is needed in the end, including the halving of rates the smoother does
//        if (mclk_frequency == 0) {
//            float rnd = fmod(target_div,1);// To make the X/Y fraction closest to 1, we set the last divider to the nearest integer to the rate divider
//            bitrate_div = int(target_div - rnd);
//            while (bitrate_div > I2S_MAX_BITRATE_DIV) { // But this might be out of range, so we right shift it into focus
//                bitrate_div >>= 1;
//            }
//            if (bitrate_div == 0) { // Could be zero, which would disable the the clock...
//                bitrate_div = 1;
//            }
//            pre_mult = float(bitrate_div) / target_div;    // Work out what we have left to correct
//            pre_num = 0;
//            pre_den = 0;
//            fraction_estimator(pre_mult, &pre_num, &pre_den);// Get the function to work out the closest fraction, there might be some point in adding some possible multipliers of these values to add to the smoothing, the reference manual (UM10360 page 480) suggests this
//
//        } else {
//            pre_mult = float(mclk_frequency * 2) / (I2S_PCLK_RATE);
//            pre_num = 0;
//            pre_den = 0;
//            fraction_estimator(pre_mult, &pre_num, &pre_den);// Get the function to work out the closest fraction, there might be some point in adding some possible multipliers of these values to add to the smoothing, the reference manual (UM10360 page 480) suggests this
//            bitrate_div = int(
//                              I2S_PCLK_RATE * float(pre_num) / float(pre_den)
//                              / float(bitrate));
//        }
//
//        old_freq = freq;
//        old_pre_num = pre_num;
//        old_pre_den = pre_den;
//        old_bitrate_div = bitrate_div;
//    } else {
//        pre_num = old_pre_num;
//        pre_den = old_pre_den;
//        bitrate_div = old_bitrate_div;
//    }

    //Clock Multiplier, MCLK setup
    if (_rxtx == I2S_TRANSMIT) {
        int regvals = ((pre_num << 8) & 0xFF00) | (pre_den & 0xFF);
//        LPC_I2S->I2STXRATE = regvals;                // Setting the X/Y fraction
//        LPC_I2S->I2STXBITRATE = (bitrate_div - 1) & 0x3F;// Setting up the bitrate divider, the periferal adds one to this

//        LPC_I2S->I2STXMODE = fourwire << 2;

        if (MasterClk_d == true) {
//            LPC_I2S->I2STXMODE |= (1 << 3);
        }
    } else {
        int regvals = ((pre_num << 8) & 0xFF00) | (pre_den & 0xFF);
//        LPC_I2S->I2SRXRATE = regvals;                // Setting the X/Y fraction
//        LPC_I2S->I2SRXBITRATE = (bitrate_div - 1) & 0x3F;// Setting up the bitrate divider, the periferal adds one to this

//        LPC_I2S->I2SRXMODE = fourwire << 2;

        if (MasterClk_d == true) {
//            LPC_I2S->I2SRXMODE |= (1 << 3);
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
//        LPC_I2S->I2SDAO = I2SDA_reg;
    } else {
        ;
//        LPC_I2S->I2SDAI = I2SDA_reg;
    }

    if (_rxtx == I2S_TRANSMIT) {
        if (txisr) {
//            LPC_I2S->I2SIRQ = (LPC_I2S->I2SIRQ & 0xFF0FFFFF)
//                              | ((interrupt_fifo_level & 0xF) << 16);
//            LPC_I2S->I2SIRQ |= 0x2;
        } else {
            ;
//            LPC_I2S->I2SIRQ &= 0xFFFFFFFD;
        }
    } else {
        if (rxisr) {
//            LPC_I2S->I2SIRQ = (LPC_I2S->I2SIRQ & 0xFFFFF0FF)
//                              | ((interrupt_fifo_level & 0xF) << 8);
//            LPC_I2S->I2SIRQ |= 0x1;
        }

        else {
            ;
//            LPC_I2S->I2SIRQ &= 0xFFFFFFFE;
        }
    }
}

void I2S::_i2sisr(void)
{
    I2STXISR.call();
    I2SRXISR.call();
}

// A function to find the nearest fraction to that put to it, with numerator and denomnator less than 256
// This is used when trying to get the clocks correct

//void I2S::fraction_estimator(float in, int * num, int * den)
//{
//    int test_num = 0;
//    int test_den = 0;
//    float least_error = 1;
//    int least_err_den = 0;
//    float genval;
//    float generr;
//
//    for (test_den = 1; test_den < I2S_MAX_DENOMINATOR; test_den++) {
//        test_num = int(float(test_den) * in);
//        if (test_num < I2S_MAX_NUMERATOR && test_num > 0) {
//            genval = float(test_num) / float(test_den);
//            generr = mod(genval - in);
//            if (generr < least_error) {
//                least_error = generr;
//                least_err_den = test_den;
//            }
//            if (generr == 0) {
//                break;
//            }
//        }
//    }
//
//    test_num = int(float(least_err_den) * in);
//    *num = test_num;
//    *den = least_err_den;
//
//}
//
//float I2S::mod(float in)
//{
//    if (in < 0)
//        in *= -1;
//
//    return in;
//}
