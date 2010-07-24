#include <eval.h>
#include <write.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/util.h>

static char is_self_evaluating(scheme_object* exp)
{
	return is_boolean(exp) ||
		is_number(exp) ||
		is_character(exp) ||
		is_string(exp);
}
static char is_tagged_list(scheme_object* exp, scheme_symbol* tag)
{
	if(is_pair(exp))
	{
		return ((scheme_symbol*) ccar(exp)) == tag;
	}
	return 0;
}
/* quote helpers*/
static char is_quoted(scheme_object* exp)
{
	return is_tagged_list(exp, quote_symbol);
}
static scheme_object* unquote(scheme_object* exp)
{
	return cadr(exp);
}

/*define and set! helpers*/
static char is_define(scheme_object* exp)
{
	return is_tagged_list(exp, define_symbol);
}
static char is_assignment(scheme_object* exp)
{
	return is_tagged_list(exp, set_bang_symbol);
}
static scheme_object* get_assignment_value(scheme_object* exp)
{
	return caddr(exp);
}
static scheme_symbol* get_assignment_variable(scheme_object* exp)
{
	return (scheme_symbol*) cadr(exp);
}
static char is_simple_define(scheme_object* exp)
{
	return is_symbol(cadr(exp));
}
static scheme_object* get_simple_define_value(scheme_object* exp)
{
	return caddr(exp);
}
static scheme_symbol* get_simple_define_variable(scheme_object* exp)
{
	return (scheme_symbol*) cadr(exp);
}
static scheme_object* make_lambda(vm* context, scheme_object* params, scheme_object* body)
{
	return cons(context, (scheme_object*)lambda_symbol, cons(context, params, body));
}
static scheme_object* make_define(vm* context, scheme_object* name, scheme_object* value)
{
	return cons(context, (scheme_object*) define_symbol,
		    cons(context, name,
			 cons(context, value, the_empty_list)));
}
//turns a complex proc definition to a simple variable assignment with lambda
static scheme_object* transform_complex_define(vm* context, scheme_object* exp)
{
	scheme_object* name = caadr(exp);
	scheme_object* params = cdadr(exp);
	scheme_object* body = cddr(exp);
	scheme_object* lambda = make_lambda(context, params, body);
	scheme_object* define = make_define(context, name, lambda);
	return define;
}
static scheme_object* eval_define(vm* context, scheme_object* exp)
{
	vm_define_variable(context,
				get_simple_define_variable(exp),
				eval(context, get_simple_define_value(exp)));
	return (scheme_object*)ok_symbol;
}
static scheme_object* eval_assignment(vm* context, scheme_object* exp)
{
	vm_set_variable_value(context,
				get_assignment_variable(exp),
				eval(context, get_assignment_value(exp)));
	return (scheme_object*)ok_symbol;
}
/*if helpers*/
static char is_if(scheme_object *exp)
{
	return is_tagged_list(exp, if_symbol);
}
static scheme_object* get_if_predicate(scheme_object* exp)
{
	return cadr(exp);
}
static scheme_object* get_if_true_case(scheme_object* exp)
{
	return caddr(exp);
}
static scheme_object* get_if_false_case(scheme_object* exp)
{
	return cadddr(exp);
}
/*procedure call helpers*/
static char is_application(scheme_object* exp)
{
	return is_pair(exp);
}
static scheme_object* get_procedure(scheme_object* exp)
{
	return ccar(exp);
}
static scheme_object* operands(scheme_object* exp)
{
	return ccdr(exp);
}
static scheme_object* first_operand(scheme_object* ops)
{
	return ccar(ops);
}
static scheme_object* rest_operands(scheme_object* ops)
{
	return ccdr(ops);
}
static char is_no_operands(scheme_object* ops)
{
	return is_the_empty_list(ops);
}
static scheme_object* list_of_values(vm* context, scheme_object* exps)
{
	if(is_no_operands(exps))
		return the_empty_list;
	scheme_object* tmp = NULL;
	gc_push_root((void**) &tmp);
	tmp = eval(context, first_operand(exps));
	tmp = cons(context,
		    tmp,
		    list_of_values(context, rest_operands(exps)));
	gc_pop_root();
	return tmp;
}
static char is_lambda(scheme_object* exp)
{
	return is_tagged_list(exp, lambda_symbol);
}
static scheme_object* get_lambda_parameters(scheme_object* exp)
{
	return cadr(exp);
}
static scheme_object* get_lambda_body(scheme_object* exp)
{
	return cddr(exp);
}
static char is_last_exp(scheme_object* exp)
{
	return is_the_empty_list(ccdr(exp));
}
static scheme_object* first_exp(scheme_object* exp)
{
	return ccar(exp);
}
static scheme_object* rest_exps(scheme_object* exps)
{
	return ccdr(exps);
}
/*begin*/
static char is_begin(scheme_object* exp)
{
	return is_tagged_list(exp, begin_symbol);
}
static scheme_object* make_begin(vm* context, scheme_object* exp)
{
	return cons(context, (scheme_object*) begin_symbol, exp);
}
static scheme_object* begin_actions(scheme_object* exp)
{
	return ccdr(exp);
}

/*cond*/
static char is_cond(scheme_object* exp)
{
	return is_tagged_list(exp, cond_symbol);
}
static scheme_object* cond_clauses(scheme_object* cond)
{
	return ccdr(cond);
}
static char is_else_clause(scheme_object* clause)
{
	return is_tagged_list(clause, else_symbol);
}
static scheme_object* sequence_to_expression(vm* context, scheme_object* seq)
{
	if(is_the_empty_list(seq))
	{
		return seq;
	}
	else if(is_last_exp(seq))
	{
		return first_exp(seq);
	}
	else
	{
		return make_begin(context, seq);
	}
}
static scheme_object* clause_actions(scheme_object* clause)
{
	return ccdr(clause);
}
static scheme_object* clause_condition(scheme_object* clause)
{
	return ccar(clause);
}
static scheme_object* make_if(vm* context, scheme_object* condition, scheme_object* true_case, scheme_object* false_case)
{
	return cons(context, (scheme_object*) if_symbol,
		    cons(context, condition,
			cons(context, true_case,
			     cons(context, false_case, (scheme_object*) the_empty_list))));
}
static scheme_object* transform_cond_clauses_to_nested_if(vm* context, scheme_object* cond_clauses)
{
	if(is_the_empty_list(cond_clauses))
	{
		return (scheme_object*) false;
	}
	else
	{
		scheme_object* clause = ccar(cond_clauses);
		scheme_object* condition = clause_condition(clause);
		scheme_object* acts = clause_actions(clause);
		scheme_object* exp = sequence_to_expression(context,acts);
		if(is_else_clause(clause))
		{
			if(is_last_exp(cond_clauses))
			{
				return exp;
			}
			else
			{
				fprintf(stderr, "else is not the last clause in the cond!\n");
				exit(1);
			}
		}
		else
		{
			gc_push_root((void**)&exp);
			scheme_object* rval = make_if(context, condition,
							exp,
							transform_cond_clauses_to_nested_if(context, ccdr(cond_clauses)));
			gc_pop_root();
			return rval;
		}		
	}
}
/*let and let helpers*/

static char is_let(scheme_object* exp)
{
	return is_tagged_list(exp, let_symbol);
}
static scheme_object* let_bindings(scheme_object* exp)
{
	return cadr(exp);
}
static scheme_object* let_body(scheme_object* exp)
{
	return cddr(exp);
}
static scheme_object* let_binding_init(scheme_object* binding)
{
	return cadr(binding);
}
static scheme_object* let_binding_var(scheme_object* binding)
{
	return ccar(binding);
}

static scheme_object* transform_let_to_lambda(vm* context, scheme_object* let)
{
	scheme_object* bindings = let_bindings(let);
	scheme_object* vars = the_empty_list;
	scheme_object* inits = the_empty_list;

	gc_push_root((void**)&vars);
	gc_push_root((void**)&inits);
	
	while(!is_the_empty_list(bindings ))
	{
		scheme_object* binding = first_exp(bindings);
		vars = cons(context, let_binding_var(binding), vars);
		inits = cons(context, let_binding_init(binding), inits);
		bindings = rest_exps(bindings );
	}
	scheme_object* tmp =  cons(context, make_lambda(context, vars, let_body(let)), inits);
	
	gc_pop_root();
	gc_pop_root();

	return tmp;
}

/*and and or*/
static char is_and(scheme_object* exp)
{
	return is_tagged_list(exp, and_symbol);
}
static char is_or(scheme_object* exp)
{
	return is_tagged_list(exp, or_symbol);
}
static scheme_object* or_tests(scheme_object* exp)
{
	return ccdr(exp);
}
static scheme_object* and_tests(scheme_object* exp)
{
	return ccdr(exp);
}
/*apply helpers*/
static scheme_object* apply_operator(scheme_object* exp)
{
	return ccar(exp);
}
static scheme_object* prepare_apply_operands(vm* context, scheme_object* ops)
{
	if(is_the_empty_list(ccdr(ops)))
	{
		return ccar(ops);
	}
	else
	{
		return cons(context, ccar(ops), prepare_apply_operands(context, ccdr(ops)));
	}
}
static scheme_object* apply_operands(vm* context, scheme_object* arguments)
{
	return prepare_apply_operands(context, ccdr(arguments));
}

/*eval helpers*/
static scheme_object* eval_expression(scheme_object* args)
{
	return ccar(args);
}
static scheme_environment* eval_environment(scheme_object* args)
{
	return (scheme_environment*) cadr(args);
}
//not so simple echo
scheme_object* eval(vm* context, scheme_object* exp)
{
	int env_pop_count = 0;
	scheme_object* rval = NULL;
	gc_push_root((void**)&exp);
	
tailcall:
	;//empty expression to fix weird label
// 	#ifdef DEBUG
// 	char* obj_str = object_to_string(exp);
// 	debug(TRACE, "Expression: %s\n", obj_str);
// 	checked_free(obj_str);
// 	#endif
	if(is_self_evaluating(exp))
	{
		rval = exp;
	}
	else if(is_if(exp))
	{
		exp = !is_false(eval(context, get_if_predicate(exp))) ? get_if_true_case(exp) : get_if_false_case(exp);
		goto tailcall;
	}
	else if(is_lambda(exp))
	{
		rval = make_compound_procedure(context, get_lambda_parameters(exp), get_lambda_body(exp));
	}
	else if(is_quoted(exp))
	{
		rval = unquote(exp);
	}
	else if(is_assignment(exp))
	{
		rval = eval_assignment(context, exp);
	}
	else if(is_define(exp))
	{
		if(!is_simple_define(exp))
		{
			exp = transform_complex_define(context, exp);
			goto tailcall;
		}
		rval = eval_define(context, exp);
	}
	else if(is_begin(exp))
	{
		exp = begin_actions(exp);
		while(!is_last_exp(exp))
		{
			eval(context, first_exp(exp));
			exp = rest_exps(exp);
		}
		exp = first_exp(exp);
		goto tailcall;
	}
	else if(is_cond(exp))
	{
		exp = transform_cond_clauses_to_nested_if(context, cond_clauses(exp));
		goto tailcall;
	}
	else if(is_let(exp))
	{
		exp = transform_let_to_lambda(context, exp);
		goto tailcall;
	}
	else if(is_or(exp))
	{
		exp = or_tests(exp);
		if(is_the_empty_list(exp))
		{
			rval = (scheme_object*) false;
		}
		else
		{
			while(!is_last_exp(exp))
			{
				scheme_object* res = eval(context, first_exp(exp));
				if(is_true(res))
				{
					rval = res;
					break;
				}
				exp = rest_exps(exp);
			}
			if(is_last_exp(exp))
			{
				exp = first_exp(exp);
				goto tailcall;
			}
		}
	}
	else if(is_and(exp))
	{
		exp = and_tests(exp);
		if(is_the_empty_list(exp))
		{
			rval = (scheme_object*) true;
		}
		else
		{
			while(!is_last_exp(exp))
			{
				scheme_object* res = eval(context, first_exp(exp));
				if(is_false(res))
				{
					rval = res;
					break;
				}
				exp = rest_exps(exp);
			}
			if(is_last_exp(exp))
			{
				exp = first_exp(exp);
				goto tailcall;
			}
		}
	}
	else if(is_variable(exp))
	{
		rval = vm_get_variable_value(context, (scheme_symbol*) exp);
	}
	else if(is_application(exp))
	{
		scheme_object* proc_exp = get_procedure(exp);
		
		scheme_object* proc = NULL;
		scheme_object* args = NULL;
		gc_push_root((void**) &proc);
		gc_push_root((void**) &args);
		
		proc = eval(context, proc_exp);
		args = list_of_values(context, operands(exp));

		//we need to special case apply
		if(is_primitive_procedure(proc))
		{
			if(is_apply(proc))
			{
				proc = apply_operator(args);
				args = apply_operands(context, args);
			}
			else if(is_eval(proc))
			{
				exp = eval_expression(args);
				vm_push_environment(context, eval_environment(args)->value);
				env_pop_count++;
				gc_pop_root();
				gc_pop_root();
				goto tailcall;
			}
		}

		if(is_primitive_procedure(proc))
		{
			scheme_primitive_procedure* pp = (scheme_primitive_procedure*) proc;
			gc_pop_root();
			gc_pop_root();
			rval = (*pp->fn)(context, args);
		}
		else
		{
			scheme_compound_procedure* cp = (scheme_compound_procedure*) proc;
			//lambda parameter application
			vm_push_environment(context, cp->env);
			env_pop_count++;
			vm_extend_environment(context, cp->arguments, args);
			exp = make_begin(context, cp->body);
			gc_pop_root();
			gc_pop_root();
			goto tailcall;
		}
	}
	else
	{
		fprintf(stderr, "Unexpected expression type: %s\n%s\n", type_to_string(exp->type), object_to_string(exp));;
		exit(1);
	}
// 	#ifdef DEBUG
// 	obj_str = object_to_string(rval);
// 	debug(TRACE, "Result: %s\n", obj_str);
// 	checked_free(obj_str);
// 	#endif
	gc_pop_root();
	for(int i = 0; i < env_pop_count; i++)
	{
		vm_pop_environment(context);
	}
	return rval;
}