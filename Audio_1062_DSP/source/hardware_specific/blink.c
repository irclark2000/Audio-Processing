/*
 * blink.c
 *
 *  Created on: Aug 25, 2024
 *      Author: isaac
 */

#include <hardware_specific/blink.h>


#define BOARD_INITPINS_IOMUXC_GPR_GPR26_GPIO_MUX1_GPIO_SEL_MASK 0x0200U /*!< GPIO1 and GPIO6 share same IO MUX function, GPIO_MUX1 selects one GPIO function: affected bits mask */


void archMix_LED_InitPins() {
	IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_10_GPIO1_IO10, 0U);
#if 0  // high speed pin option is not supported on iMX 1050
	IOMUXC_GPR->GPR26 = ((IOMUXC_GPR->GPR26 &
			(~(BOARD_INITPINS_IOMUXC_GPR_GPR26_GPIO_MUX1_GPIO_SEL_MASK)))
			| IOMUXC_GPIO_AD_B0_10_GPIO1_IO10(0x00U)
	);
#endif
	IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_08_GPIO1_IO08, GPIODEFAULTPINVALUE);
}
void LED_InitPins() {
	IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_08_GPIO1_IO08, 0U);
	IOMUXC_GPR->GPR26 = ((IOMUXC_GPR->GPR26 &
			(~(BOARD_INITPINS_IOMUXC_GPR_GPR26_GPIO_MUX1_GPIO_SEL_MASK)))
			| IOMUXC_GPR_GPR26_GPIO_MUX1_GPIO_SEL(0x00U)
	);
	IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_08_GPIO1_IO08, GPIODEFAULTPINVALUE);
}

void initializeLED(void) {
    gpio_pin_config_t led_config = {kGPIO_DigitalOutput, 0, kGPIO_NoIntmode};
    GPIO_PinInit(EXAMPLE_LED_GPIO, EXAMPLE_LED_GPIO_PIN, &led_config);
}
volatile bool g_pinSet = false;

void toggleLED(void) {
#if (defined(FSL_FEATURE_IGPIO_HAS_DR_TOGGLE) && (FSL_FEATURE_IGPIO_HAS_DR_TOGGLE == 1))
        GPIO_PortToggle(EXAMPLE_LED_GPIO, 1u << EXAMPLE_LED_GPIO_PIN);
#else
        if (g_pinSet)
        {
            GPIO_PinWrite(EXAMPLE_LED_GPIO, EXAMPLE_LED_GPIO_PIN, 0U);
            g_pinSet = false;
        }
        else
        {
            GPIO_PinWrite(EXAMPLE_LED_GPIO, EXAMPLE_LED_GPIO_PIN, 1U);
            g_pinSet = true;
        }
#endif /* FSL_FEATURE_IGPIO_HAS_DR_TOGGLE */
}
