#include <model/environment.h>
#include <stdlib.h>
#include <util/util.h>
#include <util/hashtable.h>
#include <model/model.h>
#include <string.h>

struct environment_s
{
	GC_HEADER;
	struct environment_s* parent;
	hashtable* bindings;
};

static int symbol_compare(void* data1, void* data2)
{
	scheme_symbol* sym1 = data1;
	scheme_symbol* sym2 = data2;
	
	return strcmp(sym1->value, sym2->value);
}
static unsigned int symbol_hash(void* data)
{
	scheme_symbol* sym = data;
	return string_hash(sym->value);
}

static void environment_finalize(void* data)
{
	environment* env = data;
	hashtable_destroy(env->bindings);
	debug(TRACE, "freeing environment\n");
}

struct fp_state
{
	void (*handle_pointer_cb)(void*, void*);
	void* cb_state;
};

static void find_pointers_hashtable_helper(key_value_pair* kvp, void* state)
{
	struct fp_state* fp_state = state;
	(*(fp_state->handle_pointer_cb))(kvp->key, fp_state->cb_state);
	(*(fp_state->handle_pointer_cb))(kvp->value, fp_state->cb_state);
}

static void environment_find_pointers(void* data, void (*handle_pointer_cb)(void*, void*), void *cb_state)
{
	environment* env = data;

	if(env->parent != NULL) handle_pointer_cb(env->parent, cb_state);

	struct fp_state fp_state;
	fp_state.cb_state = cb_state;
	fp_state.handle_pointer_cb = handle_pointer_cb;
	
	hashtable_foreach(env->bindings, &find_pointers_hashtable_helper, &fp_state);
}

environment* environment_create()
{
	static unsigned int gc_environment_type = 0;
	static char has_inited_gc_type = 0;
	if(!has_inited_gc_type)
	{
		has_inited_gc_type = 1;
		gc_environment_type = gc_register_type("Environment", sizeof(environment), &environment_find_pointers, &environment_finalize);
	}
	environment* nenv = gc_alloc(gc_environment_type);
	nenv->parent = NULL;
	nenv->bindings = hashtable_create(&symbol_compare, &symbol_hash);
	return nenv;
}

void environment_set_variable_value(environment* env, scheme_symbol* var, scheme_object* val)
{
	while(env != NULL)
	{
		if(hashtable_contains_key(env->bindings, var))
		{
			hashtable_put(env->bindings, var, val);
			return;
		}
		env = env->parent;
	}
	fprintf(stderr, "Unbound variable: %s\n", var->value);
}

scheme_object* environment_get_variable_value(environment* env, scheme_symbol* var)
{
	while(env != NULL)
	{
		if(hashtable_contains_key(env->bindings, var))
		{
			return (scheme_object*) hashtable_get(env->bindings, var);
		}
		env = env->parent;
	}
	fprintf(stderr, "Unbound variable: %s\n", var->value);
	exit(1);
}
void environment_define_variable(environment *env , scheme_symbol* var, scheme_object* val)
{
	hashtable_put(env->bindings, var, val);
}

environment* environment_extend(environment *env , scheme_object* vars, scheme_object* vals)
{
	environment* nenv = environment_create();
	nenv->parent = env;
	
	while(!is_the_empty_list(vars))
	{
		scheme_symbol* var = (scheme_symbol*) ccar(vars);
		scheme_object* val = ccar(vals);
		vars = ccdr(vars);
		vals = ccdr(vals);
		environment_define_variable(nenv, var, val);
	}
	return nenv;
}

environment* environment_get_top_level_environment(environment* env)
{
	while(env->parent != NULL)
	{
		env = env->parent;
	}
	return env;
}