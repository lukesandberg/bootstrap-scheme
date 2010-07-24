#ifndef __ENVIRONMENT_H__
#define __ENVIRONMENT_H__

#include <model/object.h>

typedef struct environment_s environment;
environment* environment_create();
void environment_set_variable_value(environment* env, scheme_symbol* var, scheme_object* val);
scheme_object* environment_get_variable_value(environment* env, scheme_symbol* var);
void environment_define_variable(environment *env , scheme_symbol* var, scheme_object* val);
environment* environment_extend(environment *env , scheme_object* vars, scheme_object* vals);
environment* environment_get_top_level_environment(environment* env);

#endif