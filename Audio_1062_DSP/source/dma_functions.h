/*
 * dma_functions.h
 *
 *  Created on: Mar 24, 2022
 *      Author: isaac
 */
/*
Copyright 2022 Isaac R. Clark, Jr.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation
files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy,
modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software
is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef DMA_FUNCTIONS_H_
#define DMA_FUNCTIONS_H_

#include "fsl_edma.h"
#include "fsl_dmamux.h"
#include "fsl_adc.h"

#if defined(__cplusplus)
extern "C" {
#endif


#define SYSTEM_DMA             DMA0
#define SYSTEM_DMAMUX          DMAMUX
//#define AUDIO_TX_EDMA_CHANNEL 4
//#define AUDIO_RX_EDMA_CHANNEL 5
#define POTENTIOMETER_DMA_CHANNEL0 0
#define POTENTIOMETER_DMA_CHANNEL1 1
#define POTENTIOMETER_COUNT 6


void initialize_dma_system(void);
void DMAMUX_EDMA_SetupForPotentiometers();
void ADC_TransferCreateHandleEDMA();

extern volatile uint8_t g_AdcConversionDoneFlag;
uint16_t * getADCBuffer();
void POTENTIOMETER_EDMACallback(edma_handle_t *handle, void *userData, bool transferDone, uint32_t tcds);


#if defined(__cplusplus)
}
#endif

#endif /* DMA_FUNCTIONS_H_ */
