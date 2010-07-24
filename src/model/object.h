#ifndef __OBJECT_H_
#define __OBJECT_H_

#include <gc/gc.h>
#include <util/string.h>
#include <stdio.h>

//forward declarations
struct environment_s;
struct vm_s ;

typedef enum
{
	NUMBER,
	BOOLEAN,
	CHARACTER,
	STRING,
	EMPTY_LIST,
	PAIR,
	SYMBOL,
	PRIMITIVE_PROCEDURE,
	COMPOUND_PROCEDURE,
	ENVIRONMENT,
	INPUT_PORT,
	OUTPUT_PORT,
	EOF_OBJECT
} type_t;

typedef struct
{
	GC_HEADER;
	type_t type;
} scheme_object;
typedef struct
{
	scheme_object base;
	long value;
} scheme_number;
typedef struct
{
	scheme_object base;
	char value;
} scheme_boolean;
typedef struct
{
	scheme_object base;
	char value;
} scheme_character;
typedef struct
{
	scheme_object base;
	string_t* value;
} scheme_string;
typedef struct
{
	scheme_object base;
	scheme_object* car;
	scheme_object* cdr;
} scheme_pair;
typedef struct
{
	scheme_object base;
	char* value;
} scheme_symbol;
typedef struct
{
	scheme_object base;
	scheme_object* (*fn)(struct vm_s*, scheme_object*);
} scheme_primitive_procedure;
typedef struct
{
	scheme_object base;
	scheme_object* arguments;
	scheme_object* body;
	struct environment_s* env;
	
}scheme_compound_procedure;
typedef struct
{
	scheme_object base;
	struct environment_s* value;
} scheme_environment;
typedef struct
{
	scheme_object base;
	FILE* value;
} scheme_input_port;
typedef struct
{
	scheme_object base;
	FILE* value;
} scheme_output_port;



/*global singletons*/

extern scheme_symbol* quote_symbol;
extern scheme_symbol* define_symbol;
extern scheme_symbol* set_bang_symbol;
extern scheme_symbol* ok_symbol;
extern scheme_symbol* if_symbol;
extern scheme_symbol* lambda_symbol;
extern scheme_symbol* begin_symbol;
extern scheme_symbol* cond_symbol;
extern scheme_symbol* else_symbol;
extern scheme_symbol* let_symbol;
extern scheme_symbol* and_symbol;
extern scheme_symbol* or_symbol;

extern scheme_boolean* true;
extern scheme_boolean* false;
extern scheme_object* the_empty_list;
extern scheme_object* eof;

char* object_to_string(scheme_object*);
char* type_to_string(type_t);

scheme_object* make_object(struct vm_s* context);
scheme_object* make_number(struct vm_s* context, long value);
scheme_object* make_character(struct vm_s* context, char value);
scheme_object* make_string(struct vm_s* context, string_t* str);
scheme_object* cons(struct vm_s* context, scheme_object* f, scheme_object* s);
scheme_object* make_symbol(struct vm_s* context, char* sym);
scheme_object* make_primitive_procedure(struct vm_s* context, scheme_object* (*fn)(struct vm_s* context, scheme_object *arguments));
scheme_object* make_compound_procedure(struct vm_s* context, scheme_object* parameters, scheme_object* body);
scheme_object* make_environment(struct vm_s* context, struct environment_s* env);
scheme_object* make_input_port(struct vm_s* context, FILE* stream);
scheme_object* make_output_port(struct vm_s* context, FILE* stream);


char is_number(scheme_object* obj);
char is_boolean(scheme_object* obj);
char is_true(scheme_object* obj);
char is_false(scheme_object* obj);
char is_character(scheme_object* obj);
char is_string(scheme_object* obj);
char is_the_empty_list(scheme_object* obj);
char is_pair(scheme_object* obj);
char is_symbol(scheme_object* obj);
char is_variable(scheme_object* obj);
char is_primitive_procedure(scheme_object* obj);
char is_compound_procedure(scheme_object* obj);
char is_apply(scheme_object* obj);
char is_eval(scheme_object* obj);

char is_eof(scheme_object* obj);
char is_input_port(scheme_object* obj);
char is_output_port(scheme_object* obj);
char is_environment(scheme_object* obj);

scheme_object* car(scheme_pair* p);
scheme_object* cdr(scheme_pair* p);
void set_car(scheme_pair* pair, scheme_object* nv);
void set_cdr(scheme_pair* pair, scheme_object* nv);

#define ccar(obj) car((scheme_pair*)obj)
#define ccdr(obj) cdr((scheme_pair*)obj)

#define caar(obj) ccar(ccar(obj))
#define cadr(obj) ccar(ccdr(obj))
#define cdar(obj) ccdr(ccar(obj))
#define cddr(obj) ccdr(ccdr(obj))
#define caaar(obj) ccar(ccar(ccar(obj)))
#define caadr(obj) ccar(ccar(ccdr(obj)))
#define cadar(obj) ccar(ccdr(ccar(obj)))
#define caddr(obj) ccar(ccdr(ccdr(obj)))
#define cdaar(obj) ccdr(ccar(ccar(obj)))
#define cdadr(obj) ccdr(ccar(ccdr(obj)))
#define cddar(obj) ccdr(ccdr(ccar(obj)))
#define cdddr(obj) ccdr(ccdr(ccdr(obj)))
#define caaaar(obj) ccar(ccar(ccar(ccar(obj))))
#define caaadr(obj) ccar(ccar(ccar(ccdr(obj))))
#define caadar(obj) ccar(ccar(ccdr(ccar(obj))))
#define caaddr(obj) ccar(ccar(ccdr(ccdr(obj))))
#define cadaar(obj) ccar(ccdr(ccar(ccar(obj))))
#define cadadr(obj) ccar(ccdr(ccar(ccdr(obj))))
#define caddar(obj) ccar(ccdr(ccdr(ccar(obj))))
#define cadddr(obj) ccar(ccdr(ccdr(ccdr(obj))))
#define cdaaar(obj) ccdr(ccar(ccar(ccar(obj))))
#define cdaadr(obj) ccdr(ccar(ccar(ccdr(obj))))
#define cdadar(obj) ccdr(ccar(ccdr(ccar(obj))))
#define cdaddr(obj) ccdr(ccar(ccdr(ccdr(obj))))
#define cddaar(obj) ccdr(ccdr(ccar(ccar(obj))))
#define cddadr(obj) ccdr(ccdr(ccar(ccdr(obj))))
#define cdddar(obj) ccdr(ccdr(ccdr(car(obj))))
#define cddddr(obj) ccdr(ccdr(ccdr(ccdr(obj))))

#endif