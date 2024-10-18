/*
 Demonstrates how to load a sound file and play it back using the low-level API.

 The low-level API uses a callback to deliver audio between the application and miniaudio for playback or recording. When
 in playback mode, as in this example, the application sends raw audio data to miniaudio which is then played back through
 the default playback device as defined by the operating system.

 This example uses the `ma_decoder` API to load a sound and play it back. The decoder is entirely decoupled from the
 device and can be used independently of it. This example only plays back a single sound file, but it's possible to play
 back multiple files by simple loading multiple decoders and mixing them (do not create multiple devices to do this). See
 the simple_mixing example for how best to do this.
 */

#if AUDIO_EFFECTS_TESTER
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
#include "tester_process.h"

#include <stdio.h>

static void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
    ma_decoder* pDecoder = (ma_decoder*)pDevice->pUserData;
    if (pDecoder == NULL) {
        return;
    }

    ma_decoder_read_pcm_frames(pDecoder, pOutput, frameCount, NULL);
    /* In this example the format and channel count are the same for both input and output which means we can just memcpy(). */
    //MA_COPY_MEMORY(pOutput, pInput, frameCount * ma_get_bytes_per_frame(pDevice->capture.format, pDevice->capture.channels));


    processHalf(pOutput, pOutput,
    		frameCount,
			(AUDIOFORMAT) (pDecoder->outputChannels),
			(AUDIO_DATA_TYPE) pDecoder->outputFormat,
    		pDecoder->outputSampleRate);
    (void)pInput;
}

int simple_playback()
{
    ma_result result;
    ma_decoder decoder;
    ma_device_config deviceConfig;
    ma_device device;


    char * fileName = "sounds/alan-walker-type-guitar-loop-1-246365.wav";
    result = ma_decoder_init_file(fileName, NULL, &decoder);
    if (result != MA_SUCCESS) {
        printf("Could not load file: %s\n", fileName);
        return -2;
    }
    ma_data_source_set_looping(&decoder, MA_TRUE);

    deviceConfig = ma_device_config_init(ma_device_type_playback);
    deviceConfig.playback.format   = decoder.outputFormat;
    deviceConfig.playback.channels = decoder.outputChannels;
    deviceConfig.sampleRate        = decoder.outputSampleRate;
    deviceConfig.dataCallback      = data_callback;
    deviceConfig.pUserData         = &decoder;

    if (ma_device_init(NULL, &deviceConfig, &device) != MA_SUCCESS) {
        printf("Failed to open playback device.\n");
        ma_decoder_uninit(&decoder);
        return -3;
    }
    initializeEffects(decoder.outputSampleRate);

    if (ma_device_start(&device) != MA_SUCCESS) {
        printf("Failed to start playback device.\n");
        ma_device_uninit(&device);
        ma_decoder_uninit(&decoder);
        return -4;
    }
    printf("Press Enter to quit...");
    getchar();

    ma_device_uninit(&device);
    ma_decoder_uninit(&decoder);

    return 0;
}
#endif // AUDIO_EFFECTS_TESTER
