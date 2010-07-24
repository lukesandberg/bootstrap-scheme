#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <util/util.h>
#include <util/hashtable.h>
#include <model/model.h>
#include <util/stack.h>
#include <read.h>
#include <write.h>
#include <eval.h>
#include <model/procedures.h>

environment* vm_get_current_environment(vm* context)
{
	return context->env;
}
void vm_set_variable_value(vm* context, scheme_symbol* var, scheme_object* val)
{
	environment *env = context->env;
	environment_set_variable_value(env, var, val);
}

void vm_define_variable(vm* context, scheme_symbol* var, scheme_object* val)
{
	environment *env = context->env;
	environment_define_variable(env, var, val);
}

scheme_object* vm_get_variable_value(vm* context, scheme_symbol* var)
{
	environment *env = context->env;
	return environment_get_variable_value(env, var);
}

environment* vm_get_top_level_environment(vm* context)
{
	environment* env = context->env;
	return environment_get_top_level_environment(env);
}

void vm_extend_environment(vm* context, scheme_object* vars, scheme_object* vals)
{
	environment *env = context->env;
	context->env = environment_extend(env, vars, vals);
}

void vm_push_environment(vm* context, environment* env)
{
	environment* old_env = context->env;
	stack_push(context->environment_stack, old_env);
	context->env = env;
}
void vm_pop_environment(vm* context)
{
	environment* next_env = (environment*) stack_pop(context->environment_stack);
	context->env = next_env;
}
struct fp_state
{
	void (*handle_pointer_cb)(void*, void*);
	void* cb_state;
};
static void vm_symbol_table_pointers_helper(key_value_pair* kvp, void* state)
{
	struct fp_state *fps = state;
	(*(fps->handle_pointer_cb))(kvp->value, fps->cb_state);
}
static void vm_env_stack_pointers_helper(void* ptr, void* state)
{
	struct fp_state *fps = state;
	(*(fps->handle_pointer_cb))(ptr, fps->cb_state);
}
static void vm_pointers(void* data, void (*handle_pointer_cb)(void*, void*), void *cb_state)
{
	vm* context = (vm*) data;
	struct fp_state fp_s;
	fp_s.cb_state = cb_state;
	fp_s.handle_pointer_cb = handle_pointer_cb;
	handle_pointer_cb(true, cb_state);
	handle_pointer_cb(eof, cb_state);
	handle_pointer_cb(false, cb_state);
	handle_pointer_cb(the_empty_list, cb_state);
	handle_pointer_cb(context->env, cb_state);
	/*symbol table*/
	hashtable_foreach(context->symbol_table, &vm_symbol_table_pointers_helper, &fp_s);
	stack_foreach(context->environment_stack, &vm_env_stack_pointers_helper, &fp_s);
}


vm* vm_get()
{
	static unsigned int gc_vm_type;
	static char has_inited = 0;
	static vm* v;
	if(!has_inited)
	{
		has_inited = 1;
		//we pass null for the free func because the vm will always be a root
		gc_vm_type = gc_register_type("VM", sizeof(vm),&vm_pointers, NULL);

		v = (vm*) gc_alloc(gc_vm_type);
		v->symbol_table = hashtable_create(&string_cmp, &string_hash);
		v->environment_stack = stack_create();
		
		gc_push_root((void**) &v);
		v->env = init_environment(v);

		int boolean_gc_type = gc_register_type("Scheme-Boolean", sizeof(scheme_boolean), NULL, NULL);
		
		false = gc_alloc(boolean_gc_type);
		false->base.type = BOOLEAN;
		false->value = 0;
				
		true = gc_alloc(boolean_gc_type);
		true->base.type = BOOLEAN;
		true->value = 1;
		
		the_empty_list = make_object(v);
		the_empty_list->type = EMPTY_LIST;
		
		eof = make_object(v);
		eof->type = EOF_OBJECT;
		
		/*symbols*/
		quote_symbol = (scheme_symbol*) make_symbol(v, "quote");
		define_symbol = (scheme_symbol*) make_symbol(v, "define");
		set_bang_symbol = (scheme_symbol*) make_symbol(v, "set!");
		ok_symbol = (scheme_symbol*) make_symbol(v, "ok");
		if_symbol = (scheme_symbol*) make_symbol(v, "if");
		lambda_symbol = (scheme_symbol*) make_symbol(v, "lambda");
		begin_symbol = (scheme_symbol*) make_symbol(v, "begin");
		cond_symbol = (scheme_symbol*) make_symbol(v, "cond");
		else_symbol = (scheme_symbol*) make_symbol(v, "else");
		let_symbol = (scheme_symbol*) make_symbol(v, "let");
		and_symbol = (scheme_symbol*) make_symbol(v, "and");
 		or_symbol = (scheme_symbol*) make_symbol(v, "or");
	}
	return v;
}