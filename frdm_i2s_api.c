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

// static uint32_t i2s_irq_ids[FSL_FEATURE_SOC_UART_COUNT] = {0};
static i2s_irq_handler irq_handler;
/* Array of UART peripheral base address. */
static I2S_Type *const i2s_addrs[] = I2S_BASE_PTRS;
/* Array of UART bus clock frequencies */
static clock_name_t const i2s_clocks[] = BUS_CLK;

void i2s_init(i2s_t *obj, PinName mclk, PinName wclk, PinName bclk, PinName io, SaiFunc _rxtx) {
    uint32_t i2s_tx_mclk = pinmap_peripheral(mclk, PinMap_I2S_MCLK);
    if (_rxtx == TRANSMIT) {
        uint32_t i2s_wclk = pinmap_peripheral(wclk, PinMap_I2S_TX_WCLK);
        uint32_t i2s_bclk = pinmap_peripheral(bclk, PinMap_I2S_TX_BCLK);
        uint32_t i2s_io = pinmap_peripheral(io, PinMap_I2S_TXD0);
    } else {
        uint32_t i2s_wclk = pinmap_peripheral(wclk, PinMap_I2S_RX_WCLK);
        uint32_t i2s_bclk = pinmap_peripheral(bclk, PinMap_I2S_RX_BCLK);
        uint32_t i2s_io = pinmap_peripheral(io, PinMap_I2S_RXD0);
    }

    // pin out the spi pins
    pinmap_pinout(mclk, PinMap_I2S_MCLK);
    if (_rxtx == TRANSMIT) {
        pinmap_pinout(wclk, PinMap_TX_WCLK);
        pinmap_pinout(bclk, PinMap_TX_BCLK);
        pinmap_pinout(io, PinMap_TXD0);
    } else {
        pinmap_pinout(wclk, PinMap_RX_WCLK);
        pinmap_pinout(bclk, PinMap_RX_BCLK);
        pinmap_pinout(io, PinMap_RXD0);
    }

    sai_config_t config;

    if (_rxtx == TRANSMIT) {
        SAI_TxGetDefaultConfig(&config);
        config.protocol = kSAI_BusI2S;
        // void SAI_TxInit(I2S_Type *base, const sai_config_t *config);
        SAI_TxInit(i2s_addrs[obj->instance], &config);
    } else {
        SAI_RxGetDefaultConfig(&config);
        config.protocol = kSAI_BusI2S;
        // void SAI_TxInit(I2S_Type *base, const sai_config_t *config);
        SAI_RxInit(i2s_addrs[obj->instance], &config);
    }
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

    // typedef struct _sai_config
    // {
    //     sai_protocol_t protocol;  /*!< Audio bus protocol in SAI */
    //     sai_sync_mode_t syncMode; /*!< SAI sync mode, control Tx/Rx clock sync */
    // #if defined(FSL_FEATURE_SAI_HAS_MCR) && (FSL_FEATURE_SAI_HAS_MCR)
    //     bool mclkOutputEnable;          /*!< Master clock output enable, true means master clock divider enabled */
    // #endif                              /* FSL_FEATURE_SAI_HAS_MCR */
    //     sai_mclk_source_t mclkSource;   /*!< Master Clock source */
    //     sai_bclk_source_t bclkSource;   /*!< Bit Clock source */
    //     sai_master_slave_t masterSlave; /*!< Master or slave */
    // } sai_config_t;
}

void i2s_free(i2s_t *obj) { SAI_Deinit(i2s_addrs[obj->instance]); }
