/*
 * tester_effect.h
 *
 *  Created on: Oct 18, 2024
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

#ifndef EFFECTS_TESTER_AUDIO_PLAYER_TESTER_EFFECT_H_
#define EFFECTS_TESTER_AUDIO_PLAYER_TESTER_EFFECT_H_

#include "effects/delay_based/echo.h"
#include "effect_component.h"
#include "effects_gui_interface.h"
#include "toolkit_gui.h"

int apply_effect(int source);
int play_music (char *fileName, EFFECT_COMPONENT *ec);
void stop_music_playing ();
#endif /* EFFECTS_TESTER_AUDIO_PLAYER_TESTER_EFFECT_H_ */
