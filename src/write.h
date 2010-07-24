#ifndef __WRITE_H__
#define __WRITE_H__

#include <model/model.h>

void write(vm* context, FILE* stream, scheme_object* obj);
void display(vm* context, FILE* stream, scheme_object* obj);
#endif