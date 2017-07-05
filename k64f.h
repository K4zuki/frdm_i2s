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

#ifndef K64F_H_
#define K64F_H_

#ifdef __cplusplus
extern "C" {
#endif

#if defined(TARGET_K64F)
#warning "MK64F12.h"
#include "MK64F12.h"

struct i2s_s {
    uint32_t instance;
    uint8_t bits;
};

typedef enum {
    I2S_0 = 0,
} I2SName;

const PinMap PinMap_I2S_MCLK[] = {
    {PTE6, I2S_0, 4}, {PTA17, I2S_0, 6}, {PTC6, I2S_0, 6}, {PTC8, I2S_0, 4},
};

const PinMap PinMap_I2S_TX_BCLK[] = {
    {PTE12, I2S_0, 4}, {PTA5, I2S_0, 6}, {PTB18, I2S_0, 4}, {PTC3, I2S_0, 6},
};
const PinMap PinMap_I2S_TX_WCLK[] = {
    {PTE11, I2S_0, 4}, {PTA13, I2S_0, 6}, {PTB19, I2S_0, 4}, {PTC2, I2S_0, 6},
};
const PinMap PinMap_I2S_TXD0[] = {
    {PTE10, I2S_0, 4}, {PTA12, I2S_0, 6}, {PTC1, I2S_0, 6},
};
const PinMap PinMap_I2S_TXD1[] = {
    {PTE9, I2S_0, 2}, {PTA14, I2S_0, 7}, {PTC0, I2S_0, 6},
};

const PinMap PinMap_I2S_RX_BCLK[] = {
    {PTE9, I2S_0, 4}, {PTA14, I2S_0, 6}, {PTC6, I2S_0, 4}, {PTC9, I2S_0, 4},
};
const PinMap PinMap_I2S_RX_WCLK[] = {
    {PTE8, I2S_0, 4}, {PTA16, I2S_0, 6}, {PTC7, I2S_0, 4}, {PTC10, I2S_0, 4},
};
const PinMap PinMap_I2S_RXD0[] = {
    {PTE7, I2S_0, 4}, {PTA15, I2S_0, 6}, {PTC5, I2S_0, 4},
};

const PinMap PinMap_I2S_RXD1[] = {
    {PTE8, I2S_0, 2}, {PTA16, I2S_0, 7}, {PTC11, I2S_0, 4},
};

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // TARGET_K64F
#endif  // K64F_H_
