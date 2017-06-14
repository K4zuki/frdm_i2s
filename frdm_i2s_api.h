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

#ifndef FRDM_I2S_API_H_
#define FRDM_I2S_API_H_

#include "pinmap.h"

#if defined(TARGET_K66F)
#include "k66f.h"

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

typedef struct i2s_s i2s_t;

typedef enum _sai_irq { IRQ_RX = 0, IRQ_TX } SaiIrq;
typedef enum _sai_role { MASTER = 0, SLAVE } SaiRole;
typedef enum _sai_func { TRANSMIT = 0, RECEIVE } SaiFunc;
// typedef enum { STEREO = 0, MONO } I2sChannel;
// typedef enum { MUTED = 1, UNMUTED } I2sMute;
// typedef enum { _4WIRE = 1, _3WIRE } I2sWire;
// typedef enum { RUN = 0, STOP = 1 } I2sStatus;
typedef void (*sai_irq_handler)(uint32_t id, SaiIrq event);

void i2s_init(i2s_t *obj, PinName mclk, PinName wclk, PinName bclk, PinName io, SaiFunc _rxtx);
void i2s_free(i2s_t *obj);

void i2s_samplerate(i2s_t *obj, int samplerate);
void i2s_format(i2s_t *obj, int data_bits, int mclk_Hz, int samplerate);

void i2s_irq_handler(i2s_t *obj, sai_irq_handler handler, uint32_t id);
void i2s_irq_set(i2s_t *obj, SaiIrq irq, uint32_t enable);

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // TARGET_K66F
#endif  // FRDM_I2S_API_H_
