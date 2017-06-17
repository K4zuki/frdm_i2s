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

#include "frdm_i2s_api.h"
//! #include "cmsis.h" replace by "frdm_i2s_api.h"->"MK66F18.h"
//! #include "pinmap.h" replace by "frdm_i2s_api.h"->"k66f.h"
#include "fsl_sai.h"
#include "mbed_error.h"
//! #include "peripheral_clock_defines.h" replace by "fsl_clock.h"
#include "fsl_clock.h"
//! #include "PeripheralPins.h" replace by "frdm_i2s_api.h"->"k66f.h"

static uint32_t i2s_irq_ids[FSL_FEATURE_SOC_UART_COUNT] = {0};
static sai_irq_handler irq_handler;
/* Array of UART peripheral base address. */
static I2S_Type *const i2s_addrs[] = I2S_BASE_PTRS;
/* Array of UART bus clock frequencies */
static clock_name_t const i2s_clocks[] = SAI_CLOCKS;

void i2s_default_format(sai_transfer_format_t *format);

void i2s_init(i2s_t *obj, PinName mclk, PinName wclk, PinName bclk, PinName io, int _rxtx) {
    uint32_t i2s_tx_mclk = pinmap_peripheral(mclk, PinMap_I2S_MCLK);
    uint32_t i2s_wclk;
    uint32_t i2s_bclk;
    uint32_t i2s_io;
    if (_rxtx == TRANSMIT) {
        i2s_wclk = pinmap_peripheral(wclk, PinMap_I2S_TX_WCLK);
        i2s_bclk = pinmap_peripheral(bclk, PinMap_I2S_TX_BCLK);
        i2s_io = pinmap_peripheral(io, PinMap_I2S_TXD0);
    } else {
        i2s_wclk = pinmap_peripheral(wclk, PinMap_I2S_RX_WCLK);
        i2s_bclk = pinmap_peripheral(bclk, PinMap_I2S_RX_BCLK);
        i2s_io = pinmap_peripheral(io, PinMap_I2S_RXD0);
    }

    obj->instance = pinmap_merge(i2s_io, i2s_bclk);
    MBED_ASSERT((int)obj->instance != NC);

    // pin out the spi pins
    pinmap_pinout(mclk, PinMap_I2S_MCLK);
    if (_rxtx == TRANSMIT) {
        pinmap_pinout(wclk, PinMap_I2S_TX_WCLK);
        pinmap_pinout(bclk, PinMap_I2S_TX_BCLK);
        pinmap_pinout(io, PinMap_I2S_TXD0);
    } else {
        pinmap_pinout(wclk, PinMap_I2S_RX_WCLK);
        pinmap_pinout(bclk, PinMap_I2S_RX_BCLK);
        pinmap_pinout(io, PinMap_I2S_RXD0);
    }

    sai_config_t config;
    // typedef struct _sai_config
    // {
    //     sai_bclk_source_t bclkSource;   /*!< Bit Clock source */
    //     sai_master_slave_t masterSlave; /*!< Master or slave */
    //     sai_mclk_source_t mclkSource;   /*!< Master Clock source */
    //     sai_protocol_t protocol;  /*!< Audio bus protocol in SAI */
    //     sai_sync_mode_t syncMode; /*!< SAI sync mode, control Tx/Rx clock sync */
    // #if defined(FSL_FEATURE_SAI_HAS_MCR) && (FSL_FEATURE_SAI_HAS_MCR)
    //     bool mclkOutputEnable;          /*!< Master clock output enable, true means master clock divider enabled */
    // #endif                              /* FSL_FEATURE_SAI_HAS_MCR */
    // } sai_config_t;

    if (_rxtx == TRANSMIT) {
        SAI_TxGetDefaultConfig(&config);
        // void SAI_TxGetDefaultConfig(sai_config_t *config)
        // {
        //     config->bclkSource = kSAI_BclkSourceMclkDiv;
        //     config->masterSlave = kSAI_Master;
        //     config->mclkSource = kSAI_MclkSourceSysclk;
        //     config->protocol = kSAI_BusLeftJustified;
        //     config->syncMode = kSAI_ModeAsync;
        // #if defined(FSL_FEATURE_SAI_HAS_MCR) && (FSL_FEATURE_SAI_HAS_MCR)
        //     config->mclkOutputEnable = true;
        // #endif /* FSL_FEATURE_SAI_HAS_MCR */
        // }
        config.protocol = kSAI_BusI2S;

        SAI_TxInit(i2s_addrs[obj->instance], &config);
        // void SAI_TxInit(I2S_Type *base, const sai_config_t *config);

    } else {
        SAI_RxGetDefaultConfig(&config);
        config.protocol = kSAI_BusI2S;
        // void SAI_TxInit(I2S_Type *base, const sai_config_t *config);
        SAI_RxInit(i2s_addrs[obj->instance], &config);
    }

    sai_transfer_format_t format;
    i2s_default_format(&format);
    // typedef struct _sai_transfer_format
    // {
    //     uint32_t sampleRate_Hz;   /*!< Sample rate of audio data */
    //     uint32_t bitWidth;        /*!< Data length of audio data, usually 8/16/24/32bits */
    //     sai_mono_stereo_t stereo; /*!< Mono or stereo */
    //     uint32_t masterClockHz;   /*!< Master clock frequency in Hz */
    // #if defined(FSL_FEATURE_SAI_FIFO_COUNT) && (FSL_FEATURE_SAI_FIFO_COUNT > 1)
    //     uint8_t watermark;       /*!< Watermark value */
    // #endif                       /* FSL_FEATURE_SAI_FIFO_COUNT */
    //     uint8_t channel;         /*!< Data channel used in transfer.*/
    //     sai_protocol_t protocol; /*!< Which audio protocol used */
    // } sai_transfer_format_t;

    if (_rxtx == TRANSMIT) {
        SAI_TxSetFormat(i2s_addrs[obj->instance], &format, i2s_clocks[obj->instance], format.masterClockHz);
    } else {
        SAI_RxSetFormat(i2s_addrs[obj->instance], &format, i2s_clocks[obj->instance], format.masterClockHz);
    }
    // void SAI_TxSetFormat(I2S_Type *base,
    //                      sai_transfer_format_t *format,
    //                      uint32_t mclkSourceClockHz,
    //                      uint32_t bclkSourceClockHz)
}

void i2s_free(i2s_t *obj) { SAI_Deinit(i2s_addrs[obj->instance]); }

void i2s_default_format(sai_transfer_format_t *format) {
    format->sampleRate_Hz = kSAI_SampleRate32KHz;
    format->bitWidth = kSAI_WordWidth16bits;
    format->stereo = kSAI_Stereo;
    format->masterClockHz = 12288000;
    format->watermark = 4;
    format->channel = 0;
    format->protocol = kSAI_BusI2S;
}

void i2s_format(i2s_t *obj, int _rxtx, int samplerate, int data_bits, int channel) {
    sai_transfer_format_t format;
    i2s_default_format(&format);
    format.sampleRate_Hz = samplerate;
    format.bitWidth = data_bits;
    format.stereo = channel;

    if (_rxtx == TRANSMIT) {
        SAI_TxSetFormat(i2s_addrs[obj->instance], &format, i2s_clocks[obj->instance], format.masterClockHz);
    } else {
        SAI_RxSetFormat(i2s_addrs[obj->instance], &format, i2s_clocks[obj->instance], format.masterClockHz);
    }

    i2s_samplerate(obj->instance, samplerate);
}
void i2s_samplerate(i2s_t *obj, int samplerate) {}
void i2s_irq_handler(i2s_t *obj, sai_irq_handler handler, uint32_t id) {
    irq_handler = handler;
    serial_irq_ids[obj->instance] = id;
}

void i2s_irq_set(i2s_t *obj, SaiIrq irq, uint32_t enable) { IRQn_Type uart_irqs[] = I2S_TX_IRQS; }
