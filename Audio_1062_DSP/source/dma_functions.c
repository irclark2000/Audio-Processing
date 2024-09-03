/*
 * dma_functions.c
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


#include "dma_functions.h"

volatile uint8_t g_AdcConversionDoneFlag = 0U;


AT_NONCACHEABLE_SECTION_ALIGN(static uint16_t g_adcBuffer[POTENTIOMETER_COUNT], 4);
uint16_t * getADCBuffer() {
	return g_adcBuffer;
}

uint8_t counter = 0;
void POTENTIOMETER_EDMACallback(edma_handle_t *handle, void *userData, bool transferDone, uint32_t tcds) {
	ADC_TransferCreateHandleEDMA();
#if 1
	g_AdcConversionDoneFlag |= 0x01;
#else
	if (counter == 50) {  // slow down updates for testing
		g_AdcConversionDoneFlag |= 0x01;
		counter = 0;
	}
	counter++;
#endif
}

void initialize_dma_system(void) {
    DMAMUX_Init(SYSTEM_DMAMUX);
    edma_config_t dmaConfig = {0};
    EDMA_GetDefaultConfig(&dmaConfig);
    EDMA_Init(SYSTEM_DMA, &dmaConfig);
}

edma_handle_t g_ADCEdmaHandle;
edma_transfer_config_t g_ADC_transferConfig;

void DMAMUX_EDMA_SetupForPotentiometers() {
    DMAMUX_SetSource(SYSTEM_DMAMUX, POTENTIOMETER_DMA_CHANNEL0, kDmaRequestMuxADC1);
    DMAMUX_EnableChannel(SYSTEM_DMAMUX, POTENTIOMETER_DMA_CHANNEL0);
	EDMA_CreateHandle(&g_ADCEdmaHandle, SYSTEM_DMA, POTENTIOMETER_DMA_CHANNEL0);
    EDMA_SetCallback(&g_ADCEdmaHandle, POTENTIOMETER_EDMACallback, NULL);
    ADC_TransferCreateHandleEDMA();
}

void ADC_TransferCreateHandleEDMA () {
	EDMA_PrepareTransfer(&g_ADC_transferConfig,
				(void *)0x403B0028,
				sizeof(g_adcBuffer[0]),
				g_adcBuffer,
				sizeof(g_adcBuffer[0]),
				sizeof(g_adcBuffer[0]),
				POTENTIOMETER_COUNT* 2,
				kEDMA_MemoryToMemory
				);
	EDMA_SubmitTransfer(&g_ADCEdmaHandle, &g_ADC_transferConfig);
	EDMA_StartTransfer(&g_ADCEdmaHandle);
}
