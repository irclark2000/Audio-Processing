/*
 * compatibility.h
 *
 *  Created on: Oct 13, 2024
 *      Author: isaac
 */

#ifndef COMPATIBILITY_MACROS_COMPATIBILITY_H_
#define COMPATIBILITY_MACROS_COMPATIBILITY_H_

#ifndef __NOINIT
#define __NOINIT(bank)
#endif
#ifndef AT_NONCACHEABLE_SECTION_ALIGN
#define AT_NONCACHEABLE_SECTION_ALIGN(x,y) x
#endif

#endif /* COMPATIBILITY_MACROS_COMPATIBILITY_H_ */
