/*
 * tester_effect.c
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

#if AUDIO_EFFECTS_TESTER
#include "tester_effect.h"
#include "external_ma_node.h"
#include "miniaudio.h"
#include <stdio.h>
#include "effects/delay_based/echo.h"
#include "effect_component.h"
#include "effects_gui_interface.h"
#include "toolkit_gui.h"

static ma_engine g_engine;
static ma_sound g_sound;            /* This example will play only a single sound at once, so we only need one `ma_sound` object. */
static ma_effects_node g_effects_node;   /* The echo effect is achieved using a delay node. */

int apply_effect(int source) {
    /* The engine needs to be initialized first. */
	ma_result result;
    char * fileName = 0;
    switch (source) {
    case 0:
      	 fileName = "sounds/alan-walker-type-guitar-loop-1-246365.wav";
      	 break;
    case 1:
    	 fileName = "sounds/rampb-acoustic-guitar-loop-1-70bpm-143363.wav";
    	break;
    case 2:
    	 fileName = "sounds/relaxing-guitar-128296.wav";
    	break;
    default:
    	 fileName = "sounds/alan-walker-type-guitar-loop-1-246365.wav";

    }
     EFFECT_COMPONENT *effect_under_test;
    effect_under_test = createComponent("Vibrato", 0);
    generate_gui(effects_list, 4);
	//ec.initialize(&fv, 0);
    return 0;
    result = ma_engine_init(NULL, &g_engine);
    if (result != MA_SUCCESS) {
        printf("Failed to initialize audio engine.");
        return -1;
    }
    {
        ma_effects_node_config effectsNodeConfig;
        ma_uint32 channels;
        ma_uint32 sampleRate;

        channels   = ma_engine_get_channels(&g_engine);
        sampleRate = ma_engine_get_sample_rate(&g_engine);

	// the third parameter should be of type EFFECT_COMPONENT *
        effectsNodeConfig = ma_effects_node_config_init(channels, sampleRate, effect_under_test);

        result = ma_effects_node_init(ma_engine_get_node_graph(&g_engine), &effectsNodeConfig, NULL, &g_effects_node);
        if (result != MA_SUCCESS) {
            printf("Failed to initialize effects node.");
            return -1;
        }

        /* Connect the output of the delay node to the input of the endpoint. */
        ma_node_attach_output_bus(&g_effects_node, 0, ma_engine_get_endpoint(&g_engine), 0);
    }
    /* Now we can load the sound and connect it to the delay node. */
    {
        result = ma_sound_init_from_file(&g_engine, fileName, 0, NULL, NULL, &g_sound);
        if (result != MA_SUCCESS) {
            printf("Failed to initialize sound \"%s\".", fileName);
            return -1;
        }

        /* Connect the output of the sound to the input of the effect. */
        ma_node_attach_output_bus(&g_sound, 0, &g_effects_node, 0);

        /*
        Start the sound after it's applied to the sound. Otherwise there could be a scenario where
        the very first part of it is read before the attachment to the effect is made.
        */
        ma_sound_start(&g_sound);
    }

    printf("Press Enter to quit...");
    getchar();

    ma_sound_uninit(&g_sound);
    ma_effects_node_uninit(&g_effects_node, NULL);
    ma_engine_uninit(&g_engine);
    return 0;
}
#endif
