/*
 * volume_control.h
 *
 *  Created on: Nov 12, 2024
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

#ifndef EFFECTS_COMPONENTS_VOLUME_CONTROL_H_
#define EFFECTS_COMPONENTS_VOLUME_CONTROL_H_
#include "effects_macros.h"

typedef struct {
	float maxVolume;
	float volume;
	float out;
} VOLUME;

void gui_initialize_VOLUME(VOLUME * volume);
void initialize_VOLUME (VOLUME * vol, float volume, float max_volume);
float apply_VOLUME (VOLUME * vol, float input);
void setVolume_VOLUME (VOLUME *vol, float volume);



#endif /* EFFECTS_COMPONENTS_VOLUME_CONTROL_H_ */
