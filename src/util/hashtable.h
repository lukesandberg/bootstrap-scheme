#ifndef __defined_hashtable_h
#define __defined_hashtable_h

#include <util/stack.h>

typedef struct key_value_pair_s
{
	void* key;
	void* value;
} key_value_pair;

typedef struct hashtable_s hashtable;

hashtable* hashtable_create(int (*cmp)(void*, void*), unsigned int (*hash)(void*));
void hashtable_destroy(hashtable* ht);
void* hashtable_put(hashtable* table, void* key, void* value);
void* hashtable_get(hashtable* table, void* key);
int hashtable_contains_key(hashtable* table, void* key);
void* hashtable_remove(hashtable* table, void* key);
void hashtable_foreach(hashtable* ht, void (*)(key_value_pair*, void*), void* state);

/*helpers*/
unsigned int string_hash(void* str);
int string_cmp(void*, void*);

#endif
