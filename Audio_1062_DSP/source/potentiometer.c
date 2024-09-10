/*
 * potentiometer.c
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

#include "potentiometer.h"
#include "dma_functions.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_ADC_BASE           ADC1
#define DEMO_ADC_USER_CHANNEL   16U
#define DEMO_ADC_CHANNEL_GROUP0 0U
#define DEMO_ADC_CHANNEL_GROUP1 1U
#define DEMO_ADC_CHANNEL_GROUP2 2U
#define DEMO_ADC_CHANNEL_GROUP3 3U
#define DEMO_ADC_CHANNEL_GROUP4 4U
#define DEMO_ADC_CHANNEL_GROUP5 5U

#define DEMO_ADC_ETC_BASE             ADC_ETC
#define ADC_ETC_TRIG_GRP0_CHAIN_LENGTH 4U    /* Chain length 5 */
#define ADC_ETC_TRIG_GRP1_CHAIN_LENGTH 0U    /* Chain length 1 */
#define DEMO_ADC_ETC_CHANNEL0          15U
#define DEMO_ADC_ETC_CHANNEL1          0U
#define DEMO_ADC_ETC_CHANNEL2          7U
#define DEMO_ADC_ETC_CHANNEL3          8U
#define DEMO_ADC_ETC_CHANNEL4          9U
#define DEMO_ADC_ETC_CHANNEL5          10U

#define POTENTIOMETER_ADC_ETC_DONE0_Handler ADC_ETC_IRQ0_IRQHandler
#define EXAMPLE_ADC_ETC_DONE1_Handler ADC_ETC_IRQ1_IRQHandler
#define EXAMPLE_ADC_ETC_DONE2_Handler ADC_ETC_IRQ2_IRQHandler

#define POTENTIOMETER_COUNT 6

#define DEMO_XBARA_BASE           XBARA1
#define DEMO_XBARA_INPUT_PITCH0   kXBARA1_InputPitTrigger0
#define DEMO_XBARA_OUTPUT_ADC_ETC kXBARA1_OutputAdcEtcXbar0Trig0

#define PIT_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_OscClk)

#if 1
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void ADC_Configuration(void);
static void XBARA_Configuration(void);
static void PIT_Configuration(void);
static void ADC_ETC_Setup(
		adc_etc_trigger_chain_config_t *adcEtcTriggerChainConfig,
		adc_etc_interrupt_enable_t doneInterruptTriggerEnable,
		uint8_t hcChannelSelect,
		uint8_t adcEtcChannel,
	    bool enableInterrupt,
		uint8_t index);

/*******************************************************************************
 * Variables
 ******************************************************************************/
const uint32_t g_Adc_12bitFullRange      = 4096U;
volatile uint8_t g_sampleCount = 0;
static uint16_t *s_readings;


/*******************************************************************************
 * Code
 ******************************************************************************/
#if 0
extern "C" void POTENTIOMETER_ADC_ETC_DONE0_Handler(void)
{
    g_sampleCount++;
    if (g_sampleCount >= 100) {
    	 g_AdcConversionDoneFlag |= ADC_ETC_DONE0_FLAG;
    	 g_sampleCount = 0;
    }
    ADC_ETC_ClearInterruptStatusFlags(DEMO_ADC_ETC_BASE, kADC_ETC_Trg0TriggerSource, kADC_ETC_Done0StatusFlagMask);
    /* Get trigger0 chain0 result. */
//    uint16_t * adc = getADCBuffer();
    s_readings[0] = ADC_ETC_GetADCConversionValue(DEMO_ADC_ETC_BASE, 0U, 0U);
    s_readings[1] = ADC_ETC_GetADCConversionValue(DEMO_ADC_ETC_BASE, 0U, 1U);
    s_readings[2] = ADC_ETC_GetADCConversionValue(DEMO_ADC_ETC_BASE, 0U, 2U);
    s_readings[3] = ADC_ETC_GetADCConversionValue(DEMO_ADC_ETC_BASE, 0U, 3U);
    __DSB();
}

extern "C" void EXAMPLE_ADC_ETC_DONE1_Handler(void)
{
    g_AdcConversionDoneFlag |= ADC_ETC_DONE1_FLAG;
    ADC_ETC_ClearInterruptStatusFlags(DEMO_ADC_ETC_BASE, kADC_ETC_Trg0TriggerSource, kADC_ETC_Done1StatusFlagMask);
    __DSB();
}
#endif

/*!
 * @brief Main function.
 */
void archMixPotentiometerInitPins (void) {
	//GPIO_AD_B1_04
	//GPIO_AD_B1_05
	IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_04_GPIO1_IO20, 0U);      // Channel 9
	IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_04_GPIO1_IO20, POTENTIOMETERPINVALUE);
	IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_05_GPIO1_IO21, 0U);      // Channel 10
	IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_05_GPIO1_IO21, POTENTIOMETERPINVALUE);

	//GPIO_AD_B1_06
	//GPIO_AD_B1_07
	IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_06_GPIO1_IO22, 0U);      // Channel 9
	IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_06_GPIO1_IO22, 0xB0U);
	IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_07_GPIO1_IO23, 0U);      // Channel 10
	IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_07_GPIO1_IO23, 0xB0U);
	//GPIO_AD_B1_08
	IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_08_GPIO1_IO24, 0U);      // Channel 10
	IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_08_GPIO1_IO24, 0xB0U);
}
void  potentiometerInitPins (void) {
	IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_10_GPIO1_IO26, 0U);      // Channel 15
	IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_10_GPIO1_IO26, 0xB0U);
	IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_11_GPIO1_IO27, 0U);      // Channel 0
	IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_11_GPIO1_IO27, 0xB0U);

	IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_02_GPIO1_IO18, 0U);      // Channel 7
	IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_02_GPIO1_IO18, 0xB0U);
	IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_03_GPIO1_IO19, 0U);      // Channel 8
	IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_03_GPIO1_IO19, 0xB0U);

	IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_04_GPIO1_IO20, 0U);      // Channel 9
	IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_04_GPIO1_IO20, 0xB0U);
	IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_05_GPIO1_IO21, 0U);      // Channel 10
	IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_05_GPIO1_IO21, 0xB0U);
}
void potentiometerReadInit()
{
    adc_etc_config_t adcEtcConfig;
    adc_etc_trigger_config_t adcEtcTriggerConfig;
    adc_etc_trigger_chain_config_t adcEtcTriggerChainConfig;

    /* Set PERCLK_CLK source to OSC_CLK*/
    CLOCK_SetMux(kCLOCK_PerclkMux, 1U);
    /* Set PERCLK_CLK divider to 1 */
    CLOCK_SetDiv(kCLOCK_PerclkDiv, 0U);


    PRINTF("ADC_ETC_Hardware_Trigger_Conv Example Start!\r\n");

    ADC_Configuration();
    XBARA_Configuration();
    ADC_EnableDMA(DEMO_ADC_BASE, true);
    DMAMUX_EDMA_SetupForPotentiometers();
    PIT_Configuration();
    s_readings =  getADCBuffer();

    /* Initialize the ADC_ETC. */
    ADC_ETC_GetDefaultConfig(&adcEtcConfig);
    adcEtcConfig.XBARtriggerMask = 1U; /* Enable the external XBAR trigger0. */
    adcEtcConfig.enableTSCBypass = false;
    ADC_ETC_Init(DEMO_ADC_ETC_BASE, &adcEtcConfig);

    /* Set the external XBAR trigger0 configuration. */
    adcEtcTriggerConfig.enableSyncMode      = false;
    adcEtcTriggerConfig.enableSWTriggerMode = false;
    adcEtcTriggerConfig.triggerChainLength  = ADC_ETC_TRIG_GRP0_CHAIN_LENGTH; /* Chain length is 5. */
    adcEtcTriggerConfig.triggerPriority     = 1U;
    adcEtcTriggerConfig.sampleIntervalDelay = 0U;
    adcEtcTriggerConfig.initialDelay        = 0U;
    ADC_ETC_SetTriggerConfig(DEMO_ADC_ETC_BASE, 0U, &adcEtcTriggerConfig);

    /* Set the external XBAR trigger1 configuration. */
    adcEtcTriggerConfig.triggerChainLength = ADC_ETC_TRIG_GRP1_CHAIN_LENGTH; /* Chain length is 1. */
    adcEtcTriggerConfig.triggerPriority = 0U;
    ADC_ETC_SetTriggerConfig(ADC_ETC, 1U, &adcEtcTriggerConfig);


    /* Set the external XBAR trigger0 chain configuration. */
    /* ADC1 Trigger group 0 */
    adcEtcTriggerChainConfig.enableB2BMode = true;

    adcEtcTriggerChainConfig.ADCHCRegisterSelect = 1U << DEMO_ADC_CHANNEL_GROUP0; /* Select ADC_HC0 register to trigger. */
    adcEtcTriggerChainConfig.ADCChannelSelect = DEMO_ADC_ETC_CHANNEL0; /* ADC_HC0 will be triggered to sample Corresponding channel. */
    adcEtcTriggerChainConfig.InterruptEnable = kADC_ETC_InterruptDisable; /* Disable the Done0 interrupt. */
    ADC_ETC_SetTriggerChainConfig(ADC_ETC, 0U, DEMO_ADC_CHANNEL_GROUP0, &adcEtcTriggerChainConfig); /* Configure the trigger0 chain0. */

    adcEtcTriggerChainConfig.ADCHCRegisterSelect = 1U << DEMO_ADC_CHANNEL_GROUP1; /* Select ADC_HC1 register to trigger. */
    adcEtcTriggerChainConfig.ADCChannelSelect = DEMO_ADC_ETC_CHANNEL1; /* ADC_HC1 will be triggered to sample Corresponding channel. */
    adcEtcTriggerChainConfig.InterruptEnable = kADC_ETC_InterruptDisable; /* Disable the Done1 interrupt. */
    ADC_ETC_SetTriggerChainConfig(ADC_ETC, 0U, DEMO_ADC_CHANNEL_GROUP1, &adcEtcTriggerChainConfig); /* Configure the trigger0 chain1. */

    adcEtcTriggerChainConfig.ADCHCRegisterSelect = 1U << DEMO_ADC_CHANNEL_GROUP2; /* Select ADC_HC0 register to trigger. */
    adcEtcTriggerChainConfig.ADCChannelSelect = DEMO_ADC_ETC_CHANNEL4; /* ADC_HC0 will be triggered to sample Corresponding channel. */
    adcEtcTriggerChainConfig.InterruptEnable = kADC_ETC_InterruptDisable; /* Disable the Done0 interrupt. */
    ADC_ETC_SetTriggerChainConfig(ADC_ETC, 0U, DEMO_ADC_CHANNEL_GROUP2, &adcEtcTriggerChainConfig); /* Configure the trigger0 chain0. */

    adcEtcTriggerChainConfig.ADCHCRegisterSelect = 1U << DEMO_ADC_CHANNEL_GROUP3; /* Select ADC_HC1 register to trigger. */
    adcEtcTriggerChainConfig.ADCChannelSelect = DEMO_ADC_ETC_CHANNEL5; /* ADC_HC1 will be triggered to sample Corresponding channel. */
    adcEtcTriggerChainConfig.InterruptEnable = kADC_ETC_InterruptDisable; /* Disable the Done1 interrupt. */
    ADC_ETC_SetTriggerChainConfig(ADC_ETC, 0U, DEMO_ADC_CHANNEL_GROUP3, &adcEtcTriggerChainConfig); /* Configure the trigger0 chain1. */

#if 1
     adcEtcTriggerChainConfig.ADCHCRegisterSelect = 1U << DEMO_ADC_CHANNEL_GROUP4; /* Select ADC_HC1 register to trigger. */
     adcEtcTriggerChainConfig.ADCChannelSelect = DEMO_ADC_ETC_CHANNEL2; /* ADC_HC1 will be triggered to sample Corresponding channel. */
     adcEtcTriggerChainConfig.InterruptEnable = kADC_ETC_InterruptDisable; /* Enable the Done1 interrupt. */
     ADC_ETC_SetTriggerChainConfig(ADC_ETC, 0U, DEMO_ADC_CHANNEL_GROUP4, &adcEtcTriggerChainConfig); /* Configure the trigger0 chain3. */
#endif
    /* ADC1 Trigger group 1 */

     adcEtcTriggerChainConfig.ADCHCRegisterSelect = 1U << DEMO_ADC_CHANNEL_GROUP1; /* Select ADC_HC1 register to trigger. */
     adcEtcTriggerChainConfig.ADCChannelSelect = DEMO_ADC_ETC_CHANNEL3; /* ADC_HC1 will be triggered to sample Corresponding channel. */
     adcEtcTriggerChainConfig.InterruptEnable = kADC_ETC_Done1InterruptEnable; /* Enable the Done1 interrupt. */
     ADC_ETC_SetTriggerChainConfig(ADC_ETC, 1U, DEMO_ADC_CHANNEL_GROUP1, &adcEtcTriggerChainConfig); /* Configure the trigger0 chain3. */

     /* Enable the NVIC. */
    //EnableIRQ(ADC_ETC_IRQ0_IRQn);
    EnableIRQ(ADC_ETC_IRQ1_IRQn);
    //EnableIRQ(ADC_ETC_IRQ2_IRQn);
    EnableIRQ(ADC_ETC_ERROR_IRQ_IRQn);
    /* Start PIT channel0. */
    PIT_StartTimer(PIT, kPIT_Chnl_0);
}


static void ADC_ETC_Setup(adc_etc_trigger_chain_config_t *adcEtcTriggerChainConfig,
		adc_etc_interrupt_enable_t doneInterruptTriggerEnable,
				uint8_t hcChannelSelect,
				uint8_t adcEtcChannel,
			    bool enableInterrupt,
				uint8_t index) {
	adcEtcTriggerChainConfig->ADCHCRegisterSelect = 1U
                                               << hcChannelSelect; /* Select ADC_HC0 register to trigger. */
	adcEtcTriggerChainConfig->ADCChannelSelect =
			adcEtcChannel; /* ADC_HC0 will be triggered to sample Corresponding channel. */
	adcEtcTriggerChainConfig->InterruptEnable = doneInterruptTriggerEnable; /* Enable the Done0 interrupt. */
#if defined(FSL_FEATURE_ADC_ETC_HAS_TRIGm_CHAIN_a_b_IEn_EN) && FSL_FEATURE_ADC_ETC_HAS_TRIGm_CHAIN_a_b_IEn_EN
	adcEtcTriggerChainConfig->enableIrq = enableInterrupt; /* Enable the IRQ. */
#endif                                         /* FSL_FEATURE_ADC_ETC_HAS_TRIGm_CHAIN_a_b_IEn_EN */
	ADC_ETC_SetTriggerChainConfig(DEMO_ADC_ETC_BASE, 0U, index,
                              adcEtcTriggerChainConfig); /* Configure the trigger0 chain0. */
}

/*!
 * @brief Configure ADC to working with ADC_ETC.
 */
void ADC_Configuration(void)
{
    adc_config_t k_adcConfig;
    adc_channel_config_t adcChannelConfigStruct;

    /* Initialize the ADC module. */
    ADC_GetDefaultConfig(&k_adcConfig);
    ADC_Init(DEMO_ADC_BASE, &k_adcConfig);
    ADC_EnableHardwareTrigger(DEMO_ADC_BASE, true);

    adcChannelConfigStruct.channelNumber = DEMO_ADC_USER_CHANNEL; /* External channel selection from ADC_ETC. */
    adcChannelConfigStruct.enableInterruptOnConversionCompleted = false;
    ADC_SetChannelConfig(DEMO_ADC_BASE, DEMO_ADC_CHANNEL_GROUP0, &adcChannelConfigStruct);
    ADC_SetChannelConfig(DEMO_ADC_BASE, DEMO_ADC_CHANNEL_GROUP1, &adcChannelConfigStruct);
    ADC_SetChannelConfig(DEMO_ADC_BASE, DEMO_ADC_CHANNEL_GROUP2, &adcChannelConfigStruct);
    ADC_SetChannelConfig(DEMO_ADC_BASE, DEMO_ADC_CHANNEL_GROUP3, &adcChannelConfigStruct);
    ADC_SetChannelConfig(DEMO_ADC_BASE, DEMO_ADC_CHANNEL_GROUP4, &adcChannelConfigStruct);
    ADC_SetChannelConfig(DEMO_ADC_BASE, DEMO_ADC_CHANNEL_GROUP5, &adcChannelConfigStruct);

    /* Do auto hardware calibration. */
    if (kStatus_Success == ADC_DoAutoCalibration(DEMO_ADC_BASE))
    {
        PRINTF("ADC_DoAutoCalibration() Done.\r\n");
    }
    else
    {
        PRINTF("ADC_DoAutoCalibration() Failed.\r\n");
    }
}

/*!
 * @brief Configure XBARA to work with ADC_ETC.
 */
static void XBARA_Configuration(void)
{
    /* Init xbara module. */
    XBARA_Init(DEMO_XBARA_BASE);

    /* Configure the XBARA signal connections. */
    XBARA_SetSignalsConnection(DEMO_XBARA_BASE, DEMO_XBARA_INPUT_PITCH0, DEMO_XBARA_OUTPUT_ADC_ETC);
}

/*!
 * @brief Configuration PIT to trigger ADC_ETC.
 */
static void PIT_Configuration(void)
{
    /* Structure of initialize PIT */
    pit_config_t pitConfig;

    /* Init pit module */
    PIT_GetDefaultConfig(&pitConfig);
    PIT_Init(PIT, &pitConfig);

    /* Set timer period for channel 0 */
    // about 50 times per second
    PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, USEC_TO_COUNT(8000U, PIT_SOURCE_CLOCK));
}
const float INT_TO_POTENTIOMETER = 1.0f / 4096.0f;

float getPotentiometerValue (int channel) {
	if (channel > 0 && channel <= POTENTIOMETER_COUNT)
		return s_readings[channel-1] * INT_TO_POTENTIOMETER;
	else return 0.0f;
}

#endif
