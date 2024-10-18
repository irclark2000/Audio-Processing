/*
 * tester_effect.c
 *
 *  Created on: Oct 18, 2024
 *      Author: isaac
 */

#if AUDIO_EFFECTS_TESTER
#include "tester_effect.h"
#include "miniaudio.h"
#include <stdio.h>

#define DELAY_IN_SECONDS    0.2f
#define DECAY               0.25f   /* Volume falloff for each echo. */

static ma_engine g_engine;
static ma_sound g_sound;            /* This example will play only a single sound at once, so we only need one `ma_sound` object. */
static ma_delay_node g_delayNode;   /* The echo effect is achieved using a delay node. */

void apply_effect() {
    /* The engine needs to be initialized first. */
    char * fileName = "sounds/alan-walker-type-guitar-loop-1-246365.wav";

    result = ma_engine_init(NULL, &g_engine);
    if (result != MA_SUCCESS) {
        printf("Failed to initialize audio engine.");
        return -1;
    }
    {
        ma_delay_node_config delayNodeConfig;
        ma_uint32 channels;
        ma_uint32 sampleRate;

        channels   = ma_engine_get_channels(&g_engine);
        sampleRate = ma_engine_get_sample_rate(&g_engine);

        delayNodeConfig = ma_delay_node_config_init(channels, sampleRate, (ma_uint32)(sampleRate * DELAY_IN_SECONDS), DECAY);

        result = ma_delay_node_init(ma_engine_get_node_graph(&g_engine), &delayNodeConfig, NULL, &g_delayNode);
        if (result != MA_SUCCESS) {
            printf("Failed to initialize delay node.");
            return -1;
        }

        /* Connect the output of the delay node to the input of the endpoint. */
        ma_node_attach_output_bus(&g_delayNode, 0, ma_engine_get_endpoint(&g_engine), 0);
    }
    /* Now we can load the sound and connect it to the delay node. */
    {
        result = ma_sound_init_from_file(&g_engine, fileName, 0, NULL, NULL, &g_sound);
        if (result != MA_SUCCESS) {
            printf("Failed to initialize sound \"%s\".", fileName);
            return -1;
        }

        /* Connect the output of the sound to the input of the effect. */
        ma_node_attach_output_bus(&g_sound, 0, &g_delayNode, 0);

        /*
        Start the sound after it's applied to the sound. Otherwise there could be a scenario where
        the very first part of it is read before the attachment to the effect is made.
        */
        ma_sound_start(&g_sound);
    }

    printf("Press Enter to quit...");
    getchar();

    ma_sound_uninit(&g_sound);
    ma_delay_node_uninit(&g_delayNode, NULL);
    ma_engine_uninit(&g_engine);

}
#endif
