/*
 * memory_debug.c
 *
 *  Created on: Nov 10, 2024
 *      Author: isaac
 */

#include "memory_debug.h"
#if MEMORY_DEBUGGING

void * debug_malloc(uint32_t count) {
	void * vptr = malloc(count);
	fprintf(stderr, "allocating %d at: %p\n", count, vptr);

	return vptr;
}
void * debug_free(void * vptr) {
	fprintf(stderr, "freeing: %p\n", vptr);
	if (vptr) free (vptr);
	return vptr;
}
#endif //MEMORY_DEBUGGING
