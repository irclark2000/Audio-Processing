/*
 * potentiometer.h
 *
 *  Created on: Mar 19, 2022
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

#ifndef HARDWARE_SPECIFIC_POTENTIOMETER_H_
#define HARDWARE_SPECIFIC_POTENTIOMETER_H_
#include <hardware_specific/dma_functions.h>
#include "fsl_adc.h"
#include "fsl_adc_etc.h"
#include "fsl_pit.h"
#include "fsl_xbara.h"
#include "fsl_debug_console.h"
#include "fsl_edma.h"
#include "fsl_common.h"
#include "fsl_iomuxc.h"

#define ADC_ETC_DONE0_FLAG (0x1U)
#define ADC_ETC_DONE1_FLAG (0x2U)
#define ADC_ETC_DONE2_FLAG (0x4U)

#define POTENTIOMETERPINVALUE 0xB0U
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif
void ADC_Configuration(void);
void potentiometerReadInit(void);
float getPotentiometerValue (int channel);
void  potentiometerInitPins (void);
#if defined(__cplusplus)
}
#endif

extern const uint32_t g_Adc_12bitFullRange;

#endif /* HARDWARE_SPECIFIC_POTENTIOMETER_H_ */
