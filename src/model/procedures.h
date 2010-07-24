#ifndef __PROCEDURES__H_
#define __PROCEDURES__H_

#include <model/model.h>

environment* init_environment(vm* context);
char is_apply_primitive(scheme_object* obj);
char is_eval_primitive(scheme_object* obj);

#endif