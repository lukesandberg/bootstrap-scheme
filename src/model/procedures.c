#include <model/procedures.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/util.h>

#include <write.h>
#include <read.h>
#include <eval.h>
#include <string.h>

/*primitive procedures*/

/*numeric procedures*/
static scheme_object* add_procedure(vm* context, scheme_object* args)
{
	long result = 0;
	while(!is_the_empty_list(args))
	{
		scheme_pair *p = (scheme_pair*) args;
		result += ((scheme_number*)car(p))->value;
		args = cdr(p);
	}
	return make_number(context, result);
}
static scheme_object* subtract_procedure(vm* context, scheme_object* args)
{
	scheme_pair *p = (scheme_pair*) args;
	
	long result = ((scheme_number*)car(p))->value;
	char is_unary = 1;
	args = cdr(p);
	while(!is_the_empty_list(args))
	{
		p = (scheme_pair*) args;
		is_unary = 0;
		result -= ((scheme_number*)car(p))->value;
		args = cdr(p);
	}
	return make_number(context, is_unary ? -result : result);
}
static scheme_object* multiply_procedure(vm* context, scheme_object* args)
{
	long result = 1;
	while(!is_the_empty_list(args))
	{
		scheme_pair *p = (scheme_pair*) args;
		result *= ((scheme_number*)car(p))->value;
		args = cdr(p);
	}
	return make_number(context, result);
}
static scheme_object* quotient_procedure(vm* context, scheme_object* args)
{
	scheme_pair *p = (scheme_pair*) args;
	int num = ((scheme_number*)car(p))->value;
	p = (scheme_pair*) cdr(p);
	int denom = ((scheme_number*)car(p))->value;
	return make_number(context, num/denom);
}
static scheme_object* remainder_procedure(vm* context, scheme_object* args)
{
	scheme_pair *p = (scheme_pair*) args;
	int num = ((scheme_number*)car(p))->value;
	p = (scheme_pair*) cdr(p);
	int denom = ((scheme_number*)car(p))->value;
	return make_number(context, num%denom);
}
static scheme_object* numeric_equals_procedure(vm* context, scheme_object* args)
{
	scheme_pair* p = (scheme_pair*) args;
	scheme_number* num = (scheme_number*) car(p);
	long first = num->value;
	args = cdr(p);
	while(!is_the_empty_list(args))
	{
		p = (scheme_pair*) args;
		num = (scheme_number*) car(p);
		if(num->value != first) return (scheme_object*) false;
		args = cdr(p);
	}
	return (scheme_object*) true;
}
static scheme_object* less_than_procedure(vm* context, scheme_object* args)
{
	scheme_pair* p = (scheme_pair*) args;
	scheme_number* num = (scheme_number*) car(p);
	long prev = num->value;
	args = cdr(p);
	while(!is_the_empty_list(args))
	{
		p = (scheme_pair*) args;
		num = (scheme_number*) car(p);
		long next = num->value;
		if(next <= prev) return (scheme_object*) false;
		prev = next;
		args = cdr(p);
	}
	return (scheme_object*)  true;
	
}
static scheme_object* greater_than_procedure(vm* context, scheme_object* args)
{
	scheme_pair* p = (scheme_pair*) args;
	scheme_number* num = (scheme_number*) car(p);
	long prev = num->value;
	args = cdr(p);
	while(!is_the_empty_list(args))
	{
		p = (scheme_pair*) args;
		num = (scheme_number*) car(p);
		long next = num->value;
		if(next >= prev) return (scheme_object*) false;
		prev = next;
		args = cdr(p);
	}
	return (scheme_object*) true;
}

/*type predicates*/
static scheme_object* is_empty_procedure(vm* context, scheme_object* args)
{
	return is_the_empty_list(ccar(args)) ? (scheme_object*) true : (scheme_object*) false;
}
static scheme_object* is_boolean_procedure(vm* context, scheme_object* args)
{
	return is_boolean(ccar(args)) ? (scheme_object*) true : (scheme_object*) false;
}
static scheme_object* is_symbol_procedure(vm* context, scheme_object* args)
{
	return is_symbol(ccar(args)) ? (scheme_object*) true : (scheme_object*) false;
}
static scheme_object* is_integer_procedure(vm* context, scheme_object* args)
{
	return is_number(ccar(args)) ? (scheme_object*) true : (scheme_object*) false;
}
static scheme_object* is_character_procedure(vm* context, scheme_object* args)
{
	return is_character(ccar(args)) ? (scheme_object*) true : (scheme_object*) false;
}
static scheme_object*  is_string_procedure(vm* context, scheme_object* args)
{
	return is_string(ccar(args)) ? (scheme_object*) true : (scheme_object*) false;
}
static scheme_object* is_pair_procedure(vm* context, scheme_object* args)
{
	return is_pair(ccar(args)) ? (scheme_object*) true : (scheme_object*) false;
}
static scheme_object* is_procedure_procedure(vm* context, scheme_object* args)
{
	scheme_object* obj = ccar(args);
	return (is_primitive_procedure(obj) || is_compound_procedure(obj)) ? (scheme_object*) true : (scheme_object*) false;
}
static scheme_object* is_eof_procedure(vm* context, scheme_object* args)
{
	return is_eof(ccar(args)) ? (scheme_object*) true : (scheme_object*) false;
}
static scheme_object* is_input_port_procedure(vm* context, scheme_object* args)
{
	return is_input_port(ccar(args)) ? (scheme_object*) true : (scheme_object*) false;
}
static scheme_object* is_output_port_procedure(vm* context, scheme_object* args)
{
	return is_output_port(ccar(args)) ? (scheme_object*) true : (scheme_object*) false;
}

/*type conversions*/
static scheme_object* char_to_integer_procedure(vm* context, scheme_object* args)
{
	return make_number(context, ((scheme_character*)ccar(args))->value);
}
static scheme_object* integer_to_char_procedure(vm* context, scheme_object* args)
{
	return make_character(context, ((scheme_number*)ccar(args))->value);
}
static scheme_object* integer_to_string_procedure(vm* context, scheme_object* args)
{
	long decimal = ((scheme_number*)ccar(args))->value;
	string_t* str = string_create();
	char val[20];
	snprintf(val, 20, "%ld", decimal);
	string_append(str, val);
	return make_string(context, str);
}
static scheme_object* string_to_integer_procedure(vm* context, scheme_object* args)
{
	return make_number(context, atoi(string_cstring(((scheme_string*)ccar(args))->value)));
}
static scheme_object* string_to_symbol_procedure(vm* context, scheme_object* args)
{
	return make_symbol(context, string_cstring(((scheme_string*)ccar(args))->value));
}
static scheme_object* symbol_to_string_procedure(vm* context, scheme_object* args)
{
	char* s = ((scheme_symbol*)ccar(args))->value;
	string_t* str = string_create();
	string_append(str, s);
	return make_string(context, str);
}
/*pairs and lists*/
static scheme_object* cons_procedure(vm* context, scheme_object* args)
{
	return cons(context, ccar(args), cadr(args));
}
static scheme_object* car_procedure(vm* context, scheme_object *arguments)
{
	return caar(arguments);
}

static scheme_object* cdr_procedure(vm* context, scheme_object *arguments)
{
	return cdar(arguments);
}

static scheme_object *set_car_procedure(vm* context, scheme_object *arguments)
{
	set_car((scheme_pair*)ccar(arguments),  cadr(arguments));
	return (scheme_object*)ok_symbol;
}

static scheme_object *set_cdr_procedure(vm* context, scheme_object *arguments)
{
	set_cdr((scheme_pair*)ccar(arguments), cadr(arguments));
	return (scheme_object*)ok_symbol;
}

static scheme_object *list_procedure(vm* context, scheme_object *arguments)
{
	return arguments;
}

static scheme_object*is_eq_procedure(vm* context, scheme_object *arguments)
{
	scheme_object *obj1 = ccar(arguments);
	scheme_object *obj2 = cadr(arguments);
	
	if (obj1->type != obj2->type)
	{
		return (scheme_object*) false;
	}
	switch (obj1->type) {
		case NUMBER:
			return (((scheme_number*)obj1)->value == ((scheme_number*)obj2)->value) ? (scheme_object*)true : (scheme_object*)false;
		case CHARACTER:
			return (((scheme_character*)obj1)->value == ((scheme_character*)obj2)->value) ? (scheme_object*)true : (scheme_object*)false;
		case STRING:
			return (strcmp(string_cstring(((scheme_string*)obj1)->value), string_cstring(((scheme_string*)obj2)->value)) == 0) ? (scheme_object*)true : (scheme_object*)false;
		default:
			return (obj1 == obj2) ? (scheme_object*)true : (scheme_object*)false;
	}
}

static scheme_object* read_procedure(vm* context, scheme_object* args)
{
	FILE* stream;
	if(!is_the_empty_list(args))
	{
		scheme_pair* pair = (scheme_pair*) args;
		scheme_input_port* input = (scheme_input_port*) car(pair);
		stream = input->value;
	}
	else
	{
		stream = stdin;
	}
	
	scheme_object* result = read(context, stream);
	return (result == NULL) ? eof : result;
}


static scheme_object* display_procedure(vm* context, scheme_object* args)
{
	FILE* stream;
	
	scheme_object* exp = ccar(args);
	args = ccdr(args);
	
	if(is_the_empty_list(args))
	{
		stream = stdout;
	}
	else
	{
		stream = ((scheme_output_port*)ccar(args))->value;
	}
	
	display(context, stream, exp);
	fflush(stream);
	return (scheme_object*) ok_symbol;
}

static scheme_object* write_procedure(vm* context, scheme_object* args)
{
	FILE* stream;
	
	scheme_object* exp = ccar(args);
	args = ccdr(args);
	
	if(is_the_empty_list(args))
	{
		stream = stdout;
	}
	else
	{
		stream = ((scheme_output_port*)ccar(args))->value;
	}
	
	write(context, stream, exp);
	fflush(stream);
	return (scheme_object*)ok_symbol;
}

static scheme_object* load_procedure(vm* context, scheme_object* args)
{
	char* filename = string_cstring(((scheme_string*)ccar(args))->value);
	FILE* in = fopen(filename, "r");
	if(in == NULL)
	{
		fprintf(stderr, "Could not load file \"%s\"", filename);
		exit(1);
	}
	scheme_object* exp;
	scheme_object* result;
	
	gc_push_root((void**)&result);
	
	environment* top_level = vm_get_top_level_environment(context);
	vm_push_environment(context, top_level);
	
	while((exp = read(context, in)) != NULL)
	{
		result = eval(context, exp);
	}
	
	vm_pop_environment(context);
	
 	gc_pop_root();
	fclose(in);
	
	return result;
}

static scheme_object* open_input_file_procedure(vm* context, scheme_object* args)
{
	char* filename = string_cstring(((scheme_string*)ccar(args))->value);
	FILE* in = fopen(filename, "r");
	if(in == NULL)
	{
		fprintf(stderr, "Could not open file \"%s\" for reading", filename);
		exit(1);
	}
	return make_input_port(context, in);
}
static scheme_object* open_output_file_procedure(vm* context, scheme_object* args)
{
	char* filename = string_cstring(((scheme_string*)ccar(args))->value);
	FILE* out = fopen(filename, "w");
	if(out == NULL)
	{
		fprintf(stderr, "Could not open file \"%s\" for writing", filename);
		exit(1);
	}
	return make_output_port(context, out);
}

static scheme_object* close_input_file_procedure(vm* context, scheme_object* args)
{
	FILE* stream = ((scheme_input_port*)ccar(args))->value;
	if(fclose(stream) != 0)
	{
		fprintf(stderr, "close failed!\n");
		exit(1);
	}
	return (scheme_object*)ok_symbol;
}
static scheme_object* close_output_file_procedure(vm* context, scheme_object* args)
{
	FILE* stream = ((scheme_output_port*)ccar(args))->value;
	if(fclose(stream) != 0)
	{
		fprintf(stderr, "close failed!\n");
		exit(1);
	}
	return (scheme_object*)ok_symbol;
}

static scheme_object* error_procedure(vm* context, scheme_object* args)
{
	while(!is_the_empty_list(args))
	{
		scheme_pair* p = (scheme_pair*) args;
		scheme_object* exp = car(p);
		write(context, stderr, exp);
		args = cdr(p);
	}
	fprintf(stderr, "exiting\n");
	exit(1);
}

static scheme_object* write_char_procedure(vm* context, scheme_object* args)
{
	char c = ((scheme_character*)ccar(args))->value;
	FILE* stream;
	if(is_the_empty_list(ccdr(args)))
	{
		stream = stdout;
	}
	else
	{
		stream = ((scheme_output_port*)cadr(args))->value;
	}
	if(fputc(c, stream) == EOF)
	{
		fprintf(stderr, "cannot write to stream\n");
	}
	return (scheme_object*) ok_symbol;
}
static scheme_object* read_char_procedure(vm* context, scheme_object* args)
{
	FILE* stream;	
	if(is_the_empty_list(args))
	{
		stream = stdout;
	}
	else
	{
		stream = ((scheme_output_port*)ccar(args))->value;
	}
	int c = fgetc(stream);
	if(c == EOF) return eof;
	return make_character(context, c);
}

static scheme_object* interaction_environment_procedure(vm* context, scheme_object* args)
{
	return make_environment(context, vm_get_top_level_environment(context));
}

static scheme_object* null_environment_procedure(vm* context, scheme_object* args)
{
	return make_environment(context, environment_create());
}

static scheme_object* start_environment_procedure(vm* context, scheme_object* args)
{
	return make_environment(context, init_environment(context));
}

static scheme_object* apply_procedure(vm* context, scheme_object* args)
{
	fprintf(stderr, "ERROR: should never call apply procedure.");
	exit(1);
}

static scheme_object* eval_procedure(vm* context, scheme_object* args)
{
	fprintf(stderr, "ERROR: should never call eval procedure.");
	exit(1);
}

char is_apply_primitive(scheme_object* obj)
{
	return ((scheme_primitive_procedure*)obj)->fn == apply_procedure;
}
char is_eval_primitive(scheme_object* obj)
{
	return ((scheme_primitive_procedure*)obj)->fn == eval_procedure;
}

static scheme_object* gc_info_procedure(vm* context, scheme_object* args)
{
	gc_info();
	return (scheme_object*)ok_symbol;
}

environment* init_environment(vm* context)
{
	environment* env = environment_create();
	gc_push_root((void**)&env);
	scheme_object* tmp = NULL;
	gc_push_root((void**)&tmp);
	#define declare_procedure(scheme_name, c_name)	\
				environment_define_variable(env,	\
				(scheme_symbol*)make_symbol(context, scheme_name),	\
				(tmp = make_primitive_procedure(context, c_name)))
	
	/*procedures*/
	declare_procedure("+", add_procedure);
	declare_procedure("-", subtract_procedure);
	declare_procedure("*", multiply_procedure);
	declare_procedure("=", numeric_equals_procedure);
	declare_procedure("<", less_than_procedure);
	declare_procedure(">", greater_than_procedure);
	declare_procedure("quotient", quotient_procedure);
	declare_procedure("remainder", remainder_procedure);
	
	declare_procedure("empty?", is_empty_procedure);
	declare_procedure("null?", is_empty_procedure);
	declare_procedure("boolean?", is_boolean_procedure);
	declare_procedure("symbol?", is_symbol_procedure);
	declare_procedure("integer?", is_integer_procedure);
	declare_procedure("char?", is_character_procedure);
	declare_procedure("string?", is_string_procedure);
	declare_procedure("pair?", is_pair_procedure);
	declare_procedure("list?", is_pair_procedure);
	declare_procedure("procedure?", is_procedure_procedure);
	
	declare_procedure("char->integer", char_to_integer_procedure);
	declare_procedure("integer->char", integer_to_char_procedure);
	declare_procedure("string->number", string_to_integer_procedure);
	declare_procedure("number->string", integer_to_string_procedure);
	declare_procedure("symbol->string", symbol_to_string_procedure);
	declare_procedure("string->symbol", string_to_symbol_procedure);
	
	declare_procedure("cons", cons_procedure);
	declare_procedure("car", car_procedure);
	declare_procedure("cdr", cdr_procedure);
	declare_procedure("set-car!", set_car_procedure);
	declare_procedure("set-cdr!", set_cdr_procedure);
	declare_procedure("list", list_procedure);
	
	declare_procedure("eq?", is_eq_procedure);
	
	declare_procedure("apply", apply_procedure);
	declare_procedure("eval", eval_procedure);
	
	declare_procedure("null-environment", null_environment_procedure);
	declare_procedure("environment", start_environment_procedure);
	declare_procedure("interaction-environment", interaction_environment_procedure);
	
	/*IO*/
	declare_procedure("eof?", is_eof_procedure);
	declare_procedure("input-port?", is_input_port_procedure);
	declare_procedure("output-port?", is_output_port_procedure);
	declare_procedure("read", read_procedure);
	declare_procedure("write", write_procedure);
	declare_procedure("display", display_procedure);
	declare_procedure("open-output-file", open_output_file_procedure);
	declare_procedure("open-input-file", open_input_file_procedure);
	declare_procedure("read-char", read_char_procedure);
	declare_procedure("write-char", write_char_procedure);
	declare_procedure("close-input-file", close_input_file_procedure);
	declare_procedure("close-output-file", close_output_file_procedure);
	declare_procedure("load", load_procedure);
	declare_procedure("error", error_procedure);
	declare_procedure("gc-info", gc_info_procedure);

	gc_pop_root();
	gc_pop_root();
	return env;
}
