/*
 * mixer.c
 *
 *  Created on: Sep 23, 2024
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

#include <components/mixer.h>
void initialize_MIXER (MIXER * mixer, float wet_dry) {
	setWetDry_MIXER(mixer, wet_dry);
}
float applyWetDry_MIXER (MIXER * mixer, float input_wet, float input_dry){
	return input_dry * (1.0f - mixer->wet_dry) + input_wet * mixer->wet_dry;
}
void setWetDry_MIXER (MIXER *mixer, float wet_dry){
	mixer->wet_dry = wet_dry < 0.0f ? 0.0f : (wet_dry > 1.0f ? 1.0f : wet_dry);
}
