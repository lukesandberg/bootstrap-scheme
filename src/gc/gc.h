#ifndef __GC__H_
#define __GC__H_

#include <stdlib.h>
#include <util/arraylist.h>

#define GC_HEADER gc_header ___header
struct gc_header_s
{
	unsigned int _mark;
	unsigned int _type;
};

typedef struct garbage_collector_s garbage_collector;
typedef struct gc_header_s gc_header;
typedef void (*IteratePointers)(void*, void (*)(void*, void*), void*);
typedef void (*Free)(void*);

unsigned int gc_register_type(char* name, size_t sz, IteratePointers ip, Free fr);
void* gc_alloc(unsigned int type);
void gc_push_root(void** root);
void gc_pop_root();
void gc_info();

#endif