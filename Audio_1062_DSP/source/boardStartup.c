/*
 * main.c
 *
 *  Created on: Aug 14, 2024
 *      Author: isaac
 */
#include <audio_wm8960.h>
#include "boardStartup.h"
#include "potentiometer.h"
#include "blink.h"

void board_startup() {
    BOARD_ConfigMPU();
    my_InitBootPins();
    WM8960_InitPins();
    potentiometerInitPins();
    BOARD_InitBootClocks();
    LED_InitPins();
    audio_startup();
    BOARD_InitDebugConsole();
}
// minimal setup
void my_InitBootPins() {
	  CLOCK_EnableClock(kCLOCK_Iomuxc);

	  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_12_LPUART1_TX, 0U);
	  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_13_LPUART1_RX, 0U);
	  IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_12_LPUART1_TX, GPIODEFAULTPINVALUE);
	  IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_13_LPUART1_RX, GPIODEFAULTPINVALUE);
}
