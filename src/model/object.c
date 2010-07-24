#include <model/object.h>
#include <model/model.h>
#include <util/util.h>
#include <util/hashtable.h>
#include <model/procedures.h>
#include <write.h>

#include <stdio.h>

/*syntax declarations*/
scheme_symbol* quote_symbol;
scheme_symbol* define_symbol;
scheme_symbol* set_bang_symbol;
scheme_symbol* ok_symbol;
scheme_symbol* if_symbol;
scheme_symbol* lambda_symbol;
scheme_symbol* begin_symbol;
scheme_symbol* cond_symbol;
scheme_symbol* else_symbol;
scheme_symbol* let_symbol;
scheme_symbol* and_symbol;
scheme_symbol* or_symbol;

/*singleton declarations*/
scheme_boolean* true = NULL;
scheme_boolean* false= NULL;
scheme_object* the_empty_list= NULL;
scheme_object* eof= NULL;

scheme_object* make_object(vm* context)
{
	static char has_inited = 0;
	static int gc_scheme_object_type_id;
	if(!has_inited)
	{
		has_inited = 1;
		gc_scheme_object_type_id = gc_register_type("Scheme-Object", sizeof(scheme_object), NULL, NULL);
	}
	
	scheme_object* rval = (scheme_object*) gc_alloc(gc_scheme_object_type_id);
	return rval;
}

scheme_object* make_number(vm* context, long value)
{
	static char has_inited = 0;
	static int gc_scheme_number_type_id;
	if(!has_inited)
	{
		has_inited = 1;
		gc_scheme_number_type_id = gc_register_type("Scheme-Number", sizeof(scheme_number), NULL, NULL);
	}
	scheme_number* num = gc_alloc(gc_scheme_number_type_id);
	num->base.type = NUMBER;
	num->value = value;
	return (scheme_object*) num;
}

scheme_object* make_character(vm* context, char value)
{
	static char has_inited = 0;
	static int gc_scheme_character_type_id;
	if(!has_inited)
	{
		has_inited = 1;
		gc_scheme_character_type_id = gc_register_type("Scheme-Character", sizeof(scheme_character), NULL, NULL);
	}
	scheme_character* ch = gc_alloc(gc_scheme_character_type_id);
	ch->base.type = CHARACTER;
	ch->value = value;
	return (scheme_object*) ch;
}

static void free_scheme_string(void* data)
{
	scheme_string* str = (scheme_string*) data;
	string_free(str->value);
}

scheme_object* make_string(vm* context, string_t* value)
{
	static char has_inited = 0;
	static int gc_scheme_string_type_id;
	if(!has_inited)
	{
		has_inited = 1;
		gc_scheme_string_type_id= gc_register_type("Scheme-String", sizeof(scheme_string), NULL, &free_scheme_string);
	}
	scheme_string* str= gc_alloc(gc_scheme_string_type_id);
	str->base.type = STRING;
	str->value = value;
	return (scheme_object*) str;
}
static void pair_pointers(void* data, void (*handle_pointer_cb)(void*, void*), void *cb_state)
{
	scheme_pair* pair = data;
	(*handle_pointer_cb)(pair->car, cb_state);
	(*handle_pointer_cb)(pair->cdr, cb_state);
}

scheme_object* cons(vm* context, scheme_object* f, scheme_object* s)
{
	static char has_inited = 0;
	static int gc_scheme_pair_type_id;
	if(!has_inited)
	{
		has_inited = 1;
		gc_scheme_pair_type_id= gc_register_type("Scheme-Pair", sizeof(scheme_pair), &pair_pointers, NULL);
	}
	gc_push_root((void**)&f);
	gc_push_root((void**)&s);
	
	scheme_pair* pair = gc_alloc(gc_scheme_pair_type_id);
	pair->base.type = PAIR;
	pair->car = f;
	pair->cdr = s;

	gc_pop_root();
	gc_pop_root();
	return (scheme_object*) pair;
}

static void free_scheme_symbol(void* data)
{
	scheme_symbol* sym = data;
	checked_free(sym->value);
}

scheme_object* make_symbol(vm* context, char* sym)
{
	static char has_inited = 0;
	static int gc_scheme_symbol_type_id;
	if(!has_inited)
	{
		has_inited = 1;
		gc_scheme_symbol_type_id = gc_register_type("Scheme-Symbol", sizeof(scheme_symbol), NULL, &free_scheme_symbol);
	}
	
	if(hashtable_contains_key(context->symbol_table, sym))
	{
		return (scheme_object*) hashtable_get(context->symbol_table, sym);
	}
	else
	{
		scheme_symbol* s = gc_alloc(gc_scheme_symbol_type_id);
		s->base.type = SYMBOL;
		s->value = copy_cstring(sym);

		hashtable_put(context->symbol_table, s->value, s);
		return (scheme_object*) s;
	}
}
scheme_object* make_primitive_procedure(vm* context, scheme_object* (*fn)(vm*, scheme_object*))
{
	static char has_inited = 0;
	static int gc_scheme_prim_proc_type_id;
	if(!has_inited)
	{
		has_inited = 1;
		gc_scheme_prim_proc_type_id = gc_register_type("Scheme-Primitive-Procedure", sizeof(scheme_primitive_procedure), NULL, NULL);
	}
	scheme_primitive_procedure* prim = gc_alloc(gc_scheme_prim_proc_type_id);
	prim->base.type = PRIMITIVE_PROCEDURE;
	prim->fn = fn;
	return (scheme_object*) prim;
}

static void compound_procedure_pointers(void* data, void (*handle_pointer_cb)(void*, void*), void *cb_state)
{
	scheme_compound_procedure* proc = data;
	(*handle_pointer_cb)(proc->arguments, cb_state);
	(*handle_pointer_cb)(proc->body, cb_state);
	(*handle_pointer_cb)(proc->env, cb_state);
}

scheme_object* make_compound_procedure(vm* context, scheme_object* parameters, scheme_object* body)
{
	static char has_inited = 0;
	static int gc_scheme_comp_proc_type_id;
	if(!has_inited)
	{
		has_inited = 1;
		gc_scheme_comp_proc_type_id = gc_register_type("Scheme-Compound-Procedure", sizeof(scheme_compound_procedure), &compound_procedure_pointers, NULL);
	}
	
	gc_push_root((void**)&parameters);
	gc_push_root((void**)&body);
	
	scheme_compound_procedure* p = gc_alloc(gc_scheme_comp_proc_type_id);
	p->base.type = COMPOUND_PROCEDURE;
	p->body = body;
	p->arguments = parameters;
	p->env = vm_get_current_environment(context);

	gc_pop_root();
	gc_pop_root();
	return (scheme_object*) p;
}

static void scheme_environment_pointers(void* data, void (*handle_pointer_cb)(void*, void*), void *cb_state)
{
	scheme_environment* env = data;
	(*handle_pointer_cb)(env->value, cb_state);
}

scheme_object* make_environment(vm* context, environment* env)
{
	static char has_inited = 0;
	static int gc_scheme_env_type_id;
	if(!has_inited)
	{
		has_inited = 1;
		gc_scheme_env_type_id= gc_register_type("Scheme-Environment", sizeof(scheme_environment), &scheme_environment_pointers, NULL);
	}
	gc_push_root((void**)&env);

	scheme_environment* e = gc_alloc(gc_scheme_env_type_id);
	e->base.type = ENVIRONMENT;
	e->value = env;
	
	gc_pop_root();
	return (scheme_object*) e;
}

scheme_object* make_input_port(vm* context, FILE* stream)
{
	static char has_inited = 0;
	static int gc_scheme_input_port_type_id;
	if(!has_inited)
	{
		has_inited = 1;
		gc_scheme_input_port_type_id = gc_register_type("Scheme-Input-Port", sizeof(scheme_input_port), NULL, NULL);
	}
	scheme_input_port* ip = gc_alloc(gc_scheme_input_port_type_id);
	ip->base.type = INPUT_PORT;
	ip->value = stream;
	return (scheme_object*) ip;
}
scheme_object* make_output_port(vm* context, FILE* stream)
{
	static char has_inited = 0;
	static int gc_scheme_output_port_type_id;
	if(!has_inited)
	{
		has_inited = 1;
		gc_scheme_output_port_type_id = gc_register_type("Scheme-Output-Port", sizeof(scheme_output_port), NULL, NULL);
	}
	scheme_output_port* op = gc_alloc(gc_scheme_output_port_type_id);
	op->base.type = OUTPUT_PORT;
	op->value = stream;
	return (scheme_object*) op;
}

char is_number(scheme_object *obj)
{
	return obj->type == NUMBER;
}
char is_boolean(scheme_object *obj)
{
	return obj->type == BOOLEAN;
}
char is_true(scheme_object *obj)
{
	return !is_false(obj);
}
char is_false(scheme_object *obj)
{
	return (scheme_boolean*)obj == false;
}
char is_character(scheme_object* obj)
{
	return obj->type == CHARACTER;
}
char is_string(scheme_object* obj)
{
	return obj->type == STRING;
}
char is_the_empty_list(scheme_object* obj)
{
	return obj == the_empty_list;
}
char is_pair(scheme_object* obj)
{
	return obj->type == PAIR;
}
char is_symbol(scheme_object* obj)
{
	return obj->type == SYMBOL;
}
char is_variable(scheme_object* obj)
{
	return obj->type == SYMBOL;
}
char is_primitive_procedure(scheme_object* obj)
{
	return obj->type == PRIMITIVE_PROCEDURE;
}
char is_compound_procedure(scheme_object* obj)
{
	return obj->type == COMPOUND_PROCEDURE;
}
char is_apply(scheme_object* obj)
{
	return is_apply_primitive(obj);
}
char is_eval(scheme_object* obj)
{
	return is_eval_primitive(obj);
}
char is_environment(scheme_object* obj)
{
	return obj->type == ENVIRONMENT;
}
char is_eof(scheme_object* obj)
{
	return obj == eof;
}
char is_input_port(scheme_object* obj)
{
	return obj->type == INPUT_PORT;
}
char is_output_port(scheme_object* obj)
{
	return obj->type == OUTPUT_PORT;
}
scheme_object* car(scheme_pair* p)
{
	return p->car;
}
scheme_object* cdr(scheme_pair* p)
{
	return p->cdr;
}
void set_car(scheme_pair* pair, scheme_object* nv)
{
	pair->car = nv;
}
void set_cdr(scheme_pair* pair, scheme_object* nv)
{
	pair->cdr = nv;
}

char* object_to_string(scheme_object* obj)
{
	char* rval;
	string_t *str;
	switch(obj->type)
	{
		case NUMBER:
			str = string_create();
			string_append(str, "(NUMBER: ");
			char tmp[20];
			snprintf(tmp, 20, "%ld", ((scheme_number*)obj)->value);
			string_append(str, tmp);
			string_append_char(str, ')');
			rval = copy_cstring(string_cstring(str));
			string_free(str);
			break;
		case BOOLEAN:
			if(((scheme_boolean*)obj)->value) rval = copy_cstring("(BOOLEAN: #t)");
			else rval = copy_cstring("(BOOLEAN: #f)");
			break;
		case CHARACTER:
			str = string_create();
			string_append(str, "(CHARACTER: #\\");
			char ch = ((scheme_character*)obj)->value;
			switch(ch)
			{
				case ' ':
					string_append(str, "space");
					break;
				case '\n':
					string_append(str, "newline");
					break;
				case '\t':
					string_append(str, "tab");
					break;
				default:
					string_append_char(str, ch);
			}
			string_append_char(str, ')');
			rval = copy_cstring(string_cstring(str));
			string_free(str);
			break;
		case STRING:
			str = string_create();
			string_append(str, "(STRING: \"");
			char* s = string_cstring(((scheme_string*)obj)->value);
			while(*s != '\0')
			{
				switch(*s)
				{
					case '\\':
						string_append(str, "\\\\");
						break;
					case '\n':
						string_append(str, "\\n");
						break;
					case '\t':
						string_append(str, "\\t");
						break;
					case '"':
						string_append(str, "\\\"");
						break;
					default:
						string_append_char(str, *s);
				}
				s++;
			}
			string_append(str, "\")");
			rval = copy_cstring(string_cstring(str));
			string_free(str);
			break;
		case EMPTY_LIST:
			rval = copy_cstring("()");
			break;
		case PAIR:
			;//empty statement to fix wied label requirement
			char* s1 = object_to_string(((scheme_pair*)obj)->car);
			char* s2 = object_to_string(((scheme_pair*)obj)->cdr);
			str = string_create();
			string_append_char(str, '(');
			string_append(str, s1);
			checked_free(s1);
			string_append_char(str, ' ');
			string_append(str, s2);
			checked_free(s2);
			string_append_char(str, ')');
			rval = copy_cstring(string_cstring(str));
			string_free(str);
			break;
		case SYMBOL:
			str = string_create();
			string_append(str, "(symbol: ");
			string_append(str, ((scheme_symbol*)obj)->value);
			string_append_char(str, ')');
			rval = copy_cstring(string_cstring(str));
			string_free(str);
			break;
		case PRIMITIVE_PROCEDURE:
		case COMPOUND_PROCEDURE:
			rval = copy_cstring("(#<procedure>)");
			break;
		case ENVIRONMENT:
			rval = copy_cstring("(#<environment>)");
			break;
		case INPUT_PORT:
			rval = copy_cstring("(#<input port>)");
			break;
		case OUTPUT_PORT:
			rval = copy_cstring("(#<output port>)");
			break;
		case EOF_OBJECT:
			rval = copy_cstring("(#<eof>)");
			break;
		default:
			fprintf(stderr, "Unrecognized type: %d\n", obj->type);
			CRASH_MACRO;
			exit(1);
	}
	return rval;
}

char* type_to_string(type_t type)
{
	switch(type)
	{
		case NUMBER:
			return "NUMBER";
		case BOOLEAN:
			return "BOOLEAN";
		case CHARACTER:
			return "CHARACTER";
		case STRING:
			return "STRING";
		case EMPTY_LIST:
			return "The Empty List";
		case PAIR:
			return "Pair";
		case SYMBOL:
			return "Symbol";
		case PRIMITIVE_PROCEDURE:
			return "Primitive Procedure";
		case COMPOUND_PROCEDURE:
			return "Compound Procedure";
		case ENVIRONMENT:
			return "Environment";
		case INPUT_PORT:
			return "Input Port";
		case OUTPUT_PORT:
			return "Output Port";
		case EOF_OBJECT:
			return "EOF";
		default:
			fprintf(stderr, "Unrecognized type: %d\n", type);
			CRASH_MACRO;
			exit(1);
	}
}