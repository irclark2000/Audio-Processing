/*
 ============================================================================
 Name        : Audio.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */
#if AUDIO_EFFECTS_TESTER
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "duplex_loop.h"
#include "tester_effect.h"

int main(int argc, char ** argv) {
	//simple_playback ();
	apply_effect();
	return EXIT_SUCCESS;
}
#endif //AUDIO_EFFECTS_TESTER
