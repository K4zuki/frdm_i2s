/**
* @author Giles Barton-Owen
*
* @section LICENSE
*
* Copyright (c) 2012 mbed
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
*    A I2S library for the LPC1768's built-in I2S peripheral
*
*/

#include "math.h"
#include "mbed.h"

#if defined(TARGET_MCU_K64F)
#warning "MK64F12.h"
#include "MK64F12.h"
#elif defined(TARGET_K66F)
#warning "MK66F18.h"
#include "MK66F18.h"
#endif

#ifndef I2S_H
#define I2S_H

#define I2S_TRANSMIT 0
#define I2S_RECEIVE 1

#define I2S_MASTER 0
#define I2S_SLAVE 1

#define I2S_STEREO 0
#define I2S_MONO 1

#define I2S_MUTED 1
#define I2S_UNMUTED 0

#define I2S_4WIRE 1
#define I2S_3WIRE 0

class FrdmI2s_defaults {
   public:
    static const uint32_t WORDWIDTH = 16;
    static const uint32_t SAMPLERATE = 32000;
    static const uint32_t MASTERSLAVE = I2S_MASTER;
    static const uint32_t STEREOMONO = I2S_STEREO;
    static const uint32_t MUTED = I2S_UNMUTED;
    static const uint32_t INTERRUPT_FIFO_LEVEL = 4;

    static const uint32_t MAX_DENOMINATOR = 256;
    static const uint32_t MAX_NUMERATOR = 256;
    static const uint32_t MAX_BITRATE_DIV = 64;

    static const uint32_t PCLK_RATE = 12288000;
};

/** A class to play give access to the I2S library
 *
 * - 12.288MHz MCLK sent from FRDM
 * - BitClk is BCLK for DA7212
 * - WordSelect is WCLK for DA7212
 */

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
    FrdmI2s(bool rxtx, PinName SerialData, PinName WordSelect, PinName BitClk);

    /** Destroy the I2S instance
     */
    ~FrdmI2s();

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
    void format(int mode = I2S_MASTER, int mclk = 12288000, int sample = 32000, int bit = 16);

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
    int max_fifo_points();
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
    void masterslave(bool mastermode);

    /** Switch the peripheral between different wordsizes
     *
     * @param words The number of bits per word: 8,16,32
     */
    void wordsize(int words);

    /** Define the MasterClk frequency
     *
     * @param freq The frequency desired for the MasterClk
     */
    void mclk_freq(int freq);

    /** Define the sample rate
     *
     * @param freq The desired sample rate frequency
     */
    void frequency(int freq);

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
    void stereomono(bool stereomode);

    /** Mute the peripheral
     *
     */
    void mute();

    /** Set the mute status of the peripheral
     *
     * @param mute_en Set whether the mute is enabled
     */
    void mute(bool mute_en);

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

    /** Attach a function to be called when the FIFO triggers
     *
     * @param fptr A pointer to the function to be called
     */
    void attach(void (*fptr)(void)) {
        if (_rxtx == I2S_TRANSMIT) {
            I2STXISR.attach(fptr);
            txisr = true;
        } else {
            I2SRXISR.attach(fptr);
            rxisr = true;
        }
    }

    /** Attach a member function to be called when the FIFO triggers
     *
     * @param tptr A pointer to the instance of the class
     * @param mptr A pointer to the member function
     */
    template <typename T>
    void attach(T *tptr, void (T::*mptr)(void)) {
        if (_rxtx == I2S_TRANSMIT) {
            I2STXISR.attach(tptr, mptr);
            txisr = true;
        } else {
            I2SRXISR.attach(tptr, mptr);
            rxisr = true;
        }
    }

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

    FrdmI2s_defaults defaults;
    void defaulter();

    PinName IoPin, WclkPin, BclkPin, MclkPin;
    bool WordSelect_d, BitClk_d, MasterClk_d;
    bool _rxtx;
    bool pwr;
    bool master;
    int wordwidth;
    char wordwidth_code;
    bool mclk_en;
    int mclk_frequency;
    int freq;
    bool stereo;
    bool muted;
    bool stopped;
    int interrupt_fifo_level;
    int pin_setup_err;
    int reg_write_err;
    bool deallocating;
    int old_freq;

    bool fourwire;

    int old_pre_num;
    int old_pre_den;
    int old_bitrate_div;
    static void _i2sisr(void);

    static FunctionPointer I2STXISR;
    static FunctionPointer I2SRXISR;

    static bool txisr;
    static bool rxisr;

    void write(int bufr[], int bufl[], int len);
    void read(int bufr[], int bufl[], int len);
};

#endif
