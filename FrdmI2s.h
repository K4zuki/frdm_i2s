/**
* @author K4zuki(github.com/K4zuki), originally Giles Barton-Owen
*
* MIT LICENSE
*
* Copyright (c) 2017 K4zuki(github.com/K4zuki)
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*
* @section DESCRIPTION
*    A I2S library for the NXP/Freescale FRDM's built-in I2S peripheral
*    - [ ] FRDM-K66F
*    - [ ] FRDM-K64F
*
*/

#ifndef FRDMI2S_H
#define FRDMI2S_H

#include "math.h"
#include "mbed.h"

#include "Callback.h"

#if defined(TARGET_K64F)
#warning "K64F"
#include "MK64F12.h"
// #include "frdm_i2s_api.h"
// #include "k64f.h"
#elif defined(TARGET_K66F)
#warning "K66F"
#include "frdm_i2s_api.h"
#include "k66f.h"
#endif

// class FrdmI2s_defaults {
//    public:
//     static const uint32_t WORDWIDTH = 16;
//     static const uint32_t SAMPLERATE = 32000;
//     static const uint32_t MASTERSLAVE = I2S_MASTER;
//     static const uint32_t STEREOMONO = I2S_STEREO;
//     static const uint32_t MUTED = I2S_UNMUTED;
//     static const uint32_t INTERRUPT_FIFO_LEVEL = 4;
//
//     static const uint32_t MAX_DENOMINATOR = 256;
//     static const uint32_t MAX_NUMERATOR = 256;
//     static const uint32_t MAX_BITRATE_DIV = 64;
//
//     static const uint32_t PCLK_RATE = 12288000;
// };

class FrdmI2s {
   public:
    /** Create an I2S instance
     *
     * @param rxtx     Set the I2S instance to be transmit or recieve
     * (I2S_TRANSMIT/I2S_RECEIVE)
     * @param SerialData    The serial data pin
     * @param WordSelect    The word select pin
     * @param BitClk    The clock pin
     */
    FrdmI2s(PinName SerialData, PinName WordSelect, PinName BitClk, int rxtx = FrdmI2s::TRANSMIT);
    // FrdmI2s(PinName MasterClk, PinName SerialData, PinName WordSelect, PinName BitClk, int rxtx = FrdmI2s::TRANSMIT);

    /** Destroy the I2S instance
     */
    ~FrdmI2s() {}

    enum { TRANSMIT = 0, RECEIVE };
    enum { MASTER = 0, SLAVE };
    enum { STEREO = 0, MONO };
    enum { MUTED = 1, UNMUTED };
    enum { _4WIRE = 1, _3WIRE };
    enum { RUN = 0, STOP = 1 };

    enum IrqType {
        RxIrq = 0,
        TxIrq,

        IrqCnt
    };

    /** Write to the FIFO
     *
     * @param buf[] The buffer of values to write: are bit stuffed in fours
     * @param len    The number of chars to write
     */
    void write(char buf[], int len);

    /** format whole thing
    *
    * @param mode The peripherals master/slave status (I2S_MASTER/I2S_SLAVE)
    * @param mclk The frequency desired for the MasterClk
    * @param sample The desired sample rate frequency
    * @param bit The number of bits per word: 8,16,32
    */
    void format(int role = MASTER, int mclk = 12288000, int sample = 32000, int bit = 16);
    // void format(int sample = 32000, int bit = 16,int stereo=FrdmI2s::STEREO);

    /** Write to the FIFO
     *
     * @param buf[] The buffer of values to write: are bit stuffed automatically
     * @param len    The number of chars to write
     */
    void write(int buf[], int len);

    /** Read the FIFOs contents
     *
     * @return The buffers value.
     */
    int read();

    /** Read from the FIFO
     *
     * @param buf[] The buffer of values to read: raw bit shifted
     * @param len    The number of chars to read
     */
    void read(char buf[], int len);

    /** Read from the FIFO
     *
     * @param buf[] The buffer of values to read: sorted to just values
     * @param len    The number of chars to read
     */
    void read(int buf[], int len);

    /** Get the maximum number of points of data the FIFO could store
     *
     * @return The number of points
     */
    int get_fifo_limit();

    /** Switch the peripheral on and off
     *
     * @param pwr Power status
     */
    void power(bool pwr);

    /** Switch the peripheral between master and slave
     *
     * @param mastermode The peripherals master/slave status
     * (I2S_MASTER/I2S_SLAVE)
     */
    void role(int mastermode = MASTER);

    /** Switch the peripheral between different wordsizes
     *
     * @param words The number of bits per word: 8,16,32
     */
    void wordsize(int words = 16);

    /** Define the MasterClk frequency
     *
     * @param mclk The frequency desired for the MasterClk
     */
    void mclk_freq(int mclk = 12288000);

    /** Define the sample rate
     *
     * @param wclk The desired sample rate frequency
     */
    void frequency(int wclk = 32000);

    /** Set the level that the fifo interrupts at
     *
     * @param level A number between 0 and 7 at which the fifo interrupts
     */
    void set_interrupt_fifo_level(int level);

    /** Get the current FIFO level
     *
     * @return A number between 0 and 7 the FIFO is currently at
     */
    int fifo_level();

    /** Get the current number of samples in the FIFO
     *
    * @return A number showing how many samples are in the FIFO
     */
    int fifo_points();

    /** Set whether the peripheral is in stereo or mono mode: in mono the
     * perifpheral sends out the same data twice
     *
     * @param stereomode Whether the peripheral is in stereo or mono:
     * I2S_STEREO/I2S_MONO
     */
    void stereomono(int stereomode = STEREO);

    /** Mute the peripheral
     *
     */
    void mute();

    /** Set the mute status of the peripheral
     *
     * @param mute_en Set whether the mute is enabled
     */
    void mute(int mute_en = MUTED);

    /** Stop the peripheral
     *
     */
    void stop();

    /** Start the peripheral
     *
     */
    void start();

    /** Check the Clock and Pin setup went according to plan
     *
     * @return Setup okay?
     */
    bool setup_ok();

    // static void _irq_handler(uint32_t id, SerialIrq irq_type);

    /** Attach a function to call whenever a serial interrupt is generated
     *
     *  @param func A pointer to a void function, or 0 to set as none
     *  @param type Which serial interrupt to attach the member function to (Seriall::RxIrq for receive, TxIrq for
     * transmit buffer empty)
     */
    // void attach(Callback<void()> func, IrqType type = TxIrq);

   private:
    void _set_clock_112896(void);
    void _set_clock_122800(void);
    void _i2s_init(void);
    void _i2s_set_rate(int smprate);

    void mclk_enable(bool mclk_en);

    void update_config();

    void pin_setup();

    void fraction_estimator(float in, int *num, int *den);

    float mod(float in);

    // FrdmI2s_defaults defaults;
    void defaulter();

    PinName IoPin, WclkPin, BclkPin, MclkPin;
    bool WordSelect_d, BitClk_d, MasterClk_d;

    int _rxtx;
    int _mclk;
    int _role;
    int _stereo;
    int _bit;
    int _freq;

    int _mute;
    bool pwr;
    int interrupt_fifo_level;
    int pin_setup_err;
    int reg_write_err;

    bool fourwire;

    static void _i2sisr(void);

    static FunctionPointer I2STXISR;
    static FunctionPointer I2SRXISR;

    static bool txisr;
    static bool rxisr;

    void write(int bufr[], int bufl[], int len);
    void read(int bufr[], int bufl[], int len);

    i2s_t _i2s;

   protected:
    Callback<void()> _irq[IrqCnt];

    /** Acquire exclusive access to this I2S port
     */
    virtual void lock(void);

    /** Release exclusive access to this I2S port
     */
    virtual void unlock(void);
};

#endif  // FRDMI2S_H
