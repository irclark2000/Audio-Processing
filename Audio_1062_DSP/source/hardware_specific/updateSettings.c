/*
 * updateSettings.c
 *
 *  Created on: Aug 27, 2024
 *      Author: isaac
 */
/*
Copyright 2024 Isaac R. Clark, Jr.

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

#include <hardware_specific/process.h>
#include <hardware_specific/updateSettings.h>
#define  GAININDEX 1
void setGain (void);
volatile float g_gain = 1.0;

void updateSettings() {
	setGain();
}

volatile static uint32_t update_counter = 0;
void setGain () {
	g_gain = getPotentiometerValue (GAININDEX);
	test_PROCESS (update_counter);  // simulate use of a potentiometer
	update_counter++;
	if (update_counter == 4000000000) update_counter = 0;
}