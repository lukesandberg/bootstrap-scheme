#ifndef __UTIL_H_
#define __UTIL_H_

#include <stdlib.h>

#define CRASH_MACRO (*((int*)0))++;

// #define DEBUG
typedef enum
{
	TRACE,
	WARN,
	ERROR,
	GC
} debug_level;

#define DEBUG_TYPE GC

void debug(debug_level level, const char* fmt, ...);
void* checked_malloc(const size_t sz);
void checked_free(void* ptr);
char* copy_cstring(const char*);

#endif