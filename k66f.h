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

#ifndef K66F_H_
#define K66F_H_

#if defined(TARGET_K66F)
#warning "MK66F18.h"
#include "MK66F18.h"

struct i2s_s {
    uint32_t instance;
    uint8_t bits;
};

typedef enum {
    I2S_0 = 0,
} I2SName;

const PinMap PinMap_I2S_MCLK[] = {
    {PTE6, I2S_0, 0}, {PTA17, I2S_0, 0}, {PTC6, I2S_0, 0}, {PTC8, I2S_0, 0},
};

const PinMap PinMap_I2S_TX_BCLK[] = {
    {PTE12, I2S_0, 0}, {PTA5, I2S_0, 0}, {PTB18, I2S_0, 0}, {PTC3, I2S_0, 0},
};
const PinMap PinMap_I2S_TX_WCLK[] = {
    {PTE11, I2S_0, 0}, {PTA13, I2S_0, 0}, {PTB19, I2S_0, 0}, {PTC2, I2S_0, 0},
};
const PinMap PinMap_I2S_TXD0[] = {
    {PTE10, I2S_0, 0}, {PTA12, I2S_0, 0}, {PTC1, I2S_0, 0},
};
const PinMap PinMap_I2S_TXD1[] = {
    {PTE9, I2S_0, 0}, {PTA14, I2S_0, 0}, {PTC0, I2S_0, 0},
};

const PinMap PinMap_I2S_RX_BCLK[] = {
    {PTE9, I2S_0, 0}, {PTA14, I2S_0, 0}, {PTC6, I2S_0, 0}, {PTC9, I2S_0, 0},
};
const PinMap PinMap_I2S_RX_WCLK[] = {
    {PTE8, I2S_0, 0}, {PTA16, I2S_0, 0}, {PTC7, I2S_0, 0}, {PTC10, I2S_0, 0},
};
const PinMap PinMap_I2S_RXD0[] = {
    {PTE7, I2S_0, 0}, {PTA15, I2S_0, 0}, {PTC5, I2S_0, 0},
};

const PinMap PinMap_I2S_RXD1[] = {
    {PTE8, I2S_0, 0}, {PTA16, I2S_0, 0}, {PTC11, I2S_0, 0},
};

#endif  // TARGET_K66F
#endif  // K66F_H_
