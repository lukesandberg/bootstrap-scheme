
#ifndef __defined_arraylist_h
#define __defined_arraylist_h

#include <stdlib.h>
#include <stdio.h>

typedef struct arraylist_Struct arraylist;

/*
  function declarations
*/
void arraylist_free(arraylist* list);
arraylist* arraylist_create();
int arraylist_add(arraylist* list, void* object);
int arraylist_remove(arraylist* list, const void* object);
int arraylist_contains(arraylist* list, const void* object);
int arraylist_index_of(arraylist* list, const void* object);
int arraylist_is_empty(arraylist* list);
int arraylist_size(arraylist* list);
void* arraylist_get(arraylist* list, const int index);
void arraylist_clear(arraylist* list);
void arraylist_sort(arraylist* list, int (*compare)(const void*, const void*));


#endif /* __defined_arraylist_h */
