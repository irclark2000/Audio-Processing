/*
 * memory_debug.h
 *
 *  Created on: Nov 10, 2024
 *      Author: isaac
 */

#ifndef EFFECTS_COMPONENTS_MEMORY_DEBUG_H_
#define EFFECTS_COMPONENTS_MEMORY_DEBUG_H_
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#if MEMORY_DEBUGGING
#define MALLOC(x) debug_malloc(x)
#define FREE(x) debug_free(x)
#else
#define MALLOC(x) malloc(x)
#define FREE(x) free(x)
#endif

void * debug_malloc(uint32_t count);
void * debug_free(void * vptr);


#endif /* EFFECTS_COMPONENTS_MEMORY_DEBUG_H_ */
