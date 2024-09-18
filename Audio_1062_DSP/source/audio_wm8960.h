/*
 * audio_wm8960.h
 *
 *  Created on: Aug 13, 2024
 *      Author: isaac
 */

#ifndef AUDIO_WM8960_H_
#define AUDIO_WM8960_H_

//#include "pin_mux.h"
//#include "clock_config.h"
#include "board.h"

#include "fsl_debug_console.h"

#include "fsl_sai_edma.h"
#include "fsl_codec_common.h"
#include "fsl_wm8960.h"
#include "fsl_codec_adapter.h"
#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "fsl_sai_edma.h"
#include "fsl_dmamux.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* SAI instance and clock */
#define USE_NEW_SAI 1         // 1 = half complete interrupt ping-pong
#define AUD_CODEC_WM8960
#define AUD_SAI              SAI1
#define AUD_SAI_CHANNEL      (0)
#define AUD_SAI_IRQ          SAI1_IRQn
#define AUD_SAITxIRQHandler  SAI1_IRQHandler
#define AUD_SAI_TX_SYNC_MODE kSAI_ModeAsync
#define AUD_SAI_RX_SYNC_MODE kSAI_ModeSync
#define AUD_SAI_MCLK_OUTPUT  true
#define AUD_SAI_MASTER_SLAVE kSAI_Master
#define AUD_SAI_TX_BIT_CLOCK_POLARITY kSAI_PolarityActiveLow

#define AUD_AUDIO_DATA_CHANNEL (2U)
#define AUD_AUDIO_BIT_WIDTH    kSAI_WordWidth16bits
#define AUD_AUDIO_SAMPLE_RATE  (kSAI_SampleRate48KHz)
#define AUD_AUDIO_MASTER_CLOCK AUD_SAI_CLK_FREQ
#define AUD_WM8960_MCLK 12288000U

/* IRQ */
#define AUD_SAI_TX_IRQ SAI1_IRQn
#define AUD_SAI_RX_IRQ SAI1_IRQn

/* DMA */
#define AUD_DMA             DMA0
#define AUD_DMAMUX          DMAMUX
#define AUD_AUDIO_TX_EDMA_CHANNEL (4U)
#define AUD_AUDIO_RX_EDMA_CHANNEL (5U)
#define AUD_SAI_TX_SOURCE   kDmaRequestMuxSai1Tx
#define AUD_SAI_RX_SOURCE   kDmaRequestMuxSai1Rx


/* Select Audio/Video PLL (786.48 MHz) as sai1 clock source */
#define AUD_SAI1_CLOCK_SOURCE_SELECT (2U)
/* Clock pre divider for sai1 clock source */
#define AUD_SAI1_CLOCK_SOURCE_PRE_DIVIDER (0U)
/* Clock divider for sai1 clock source */
#define AUD_SAI1_CLOCK_SOURCE_DIVIDER (63U)
/* Get frequency of sai1 clock */
#define AUD_SAI_CLK_FREQ                                                        \
    (CLOCK_GetFreq(kCLOCK_AudioPllClk) / (AUD_SAI1_CLOCK_SOURCE_DIVIDER + 1U) / \
     (AUD_SAI1_CLOCK_SOURCE_PRE_DIVIDER + 1U))


/* I2C instance and clock */
#define AUD_I2C LPI2C1

/* Select USB1 PLL (480 MHz) as master lpi2c clock source */
#define AUD_LPI2C_CLOCK_SOURCE_SELECT (0U)
/* Clock divider for master lpi2c clock source */
#define AUD_LPI2C_CLOCK_SOURCE_DIVIDER (5U)
/* Get frequency of lpi2c clock */
#define AUD_I2C_CLK_FREQ ((CLOCK_GetFreq(kCLOCK_Usb1PllClk) / 8) / (AUD_LPI2C_CLOCK_SOURCE_DIVIDER + 1U))

#define BOARD_MASTER_CLOCK_CONFIG()
#define HALF_BUFFER_SIZE   (512U)
//#define BUFFER_NUMBER (2U)
extern volatile bool g_Transfer_Done;
#ifndef AUD_CODEC_INIT_DELAY_MS
#define AUD_CODEC_INIT_DELAY_MS (1000U)
#endif


#ifndef AUD_CODEC_VOLUME
#define AUD_CODEC_VOLUME 100U
#endif

#if defined(__cplusplus)
extern "C" {
#endif
void WM8960_InitPins(void);
void audio_startup(void);
uint8_t * getBuffer (void);
uint32_t getTX_Index (void);
void setTX_Index(uint32_t indx);
void incrementTX_Index(void);
uint32_t getRX_Index (void);
void setRX_Index(uint32_t indx);
void incrementRX_Index(void);
void setup_SAI_EDMA(void);
extern volatile uint32_t emptyBlock;
extern sai_edma_handle_t txHandle;
extern sai_edma_handle_t rxHandle;
extern void * g_bufIn;
extern void * g_bufOut;

#if defined(__cplusplus)
}
#endif


#endif /* AUDIO_WM8960_H_ */
