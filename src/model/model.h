#ifndef __MODEL_H__
#define __MODEL_H__

#include <stdio.h>

#include <gc/gc.h>
#include <model/environment.h>
#include <model/object.h>
#include <util/hashtable.h>
#include <util/stack.h>

typedef struct vm_s
{
	GC_HEADER;
	/*Global Variables and singletons*/
	hashtable* symbol_table;
	environment* env;
	stack* environment_stack;
} vm;

vm* vm_get();

environment* vm_get_current_environment(vm* context);
environment* vm_get_top_level_environment(vm* context);

void vm_set_variable_value(vm* context, scheme_symbol* var, scheme_object* val);
void vm_define_variable(vm* context, scheme_symbol* var, scheme_object* val);
scheme_object* vm_get_variable_value(vm* context, scheme_symbol* var);

void vm_extend_environment(vm* context, scheme_object* vars, scheme_object* vals);

void vm_pop_environment(vm* context);
void vm_push_environment(vm* context, environment* env);

#endif