#include <stdio.h>
#include <stdlib.h>
#include <util/util.h>
#include <write.h>

static void write_pair(vm* context, FILE* stream, scheme_pair* pair)
{
	write(context, stream, car(pair));
	scheme_object* s = cdr(pair);
	if(is_pair(s))
	{
		putc(' ', stream);
		write_pair(context, stream, (scheme_pair*) s);
	}
	else if(!is_the_empty_list(s))
	{
		putc(' ', stream);
		putc('.', stream);
		putc(' ', stream);
		write(context, stream, s);
	}
}

static void display_pair(vm* context, FILE* stream, scheme_pair* pair)
{
	display(context, stream, car(pair));
	scheme_object* s = cdr(pair);
	if(is_pair(s))
	{
		putc(' ', stream);
		display_pair(context, stream, (scheme_pair*) s);
	}
	else if(!is_the_empty_list(s))
	{
		putc(' ', stream);
		putc('.', stream);
		putc(' ', stream);
		display(context, stream, s);
	}
}

void display(vm* context, FILE* stream, scheme_object* obj)
{
	switch(obj->type)
	{
		case NUMBER:
			fprintf(stream, "%ld", ((scheme_number*) obj)->value);
			break;
		case BOOLEAN:
			fprintf(stream, "%s", ((scheme_boolean*) obj)->value? "true" : "false");
			break;
		case CHARACTER:
			putc(((scheme_character*) obj)->value, stream);
			break;
		case STRING:
			fprintf(stream, "%s", string_cstring(((scheme_string*) obj)->value));
			break;
		case EMPTY_LIST:
			fprintf(stream, "()");
			break;
		case PAIR:
			putc('(', stream);
			display_pair(context, stream, (scheme_pair*)obj);
			putc(')', stream);
			break;
		case SYMBOL:
			fputs(((scheme_symbol*) obj)->value, stream);
			break;
		case PRIMITIVE_PROCEDURE:
		case COMPOUND_PROCEDURE:
			fprintf(stream, "(#<procedure>)");
			break;
		case ENVIRONMENT:
			fprintf(stream, "(#<environment>)");
			break;
		case INPUT_PORT:
			fprintf(stream, "(#<input port>)");
			break;
		case OUTPUT_PORT:
			fprintf(stream, "(#<output port>)");
			break;
		case EOF_OBJECT:
			fprintf(stream, "(#<eof>)");
			break;
		default:
			fprintf(stderr, "Unrecognized type: %d\n", obj->type);
			CRASH_MACRO;
			exit(1);
	}
}


void write(vm* context, FILE* stream, scheme_object* obj)
{
	switch(obj->type)
	{
		case NUMBER:
			fprintf(stream, "%ld", ((scheme_number*) obj)->value);
			break;
		case BOOLEAN:
			fprintf(stream, "#%c", ((scheme_boolean*) obj)->value? 't' : 'f');
			break;
		case CHARACTER:
			fprintf(stream, "#\\");
			char ch = ((scheme_character*) obj)->value;
			switch(ch)
			{
				case ' ':
					fprintf(stream, "space");
					break;
				case '\n':
					fprintf(stream, "newline");
					break;
				case '\t':
					fprintf(stream, "tab");
					break;
				default:
					putc(ch, stream);
			}
			break;
		case STRING:
			putc('"', stream);
			char* str = string_cstring(((scheme_string*) obj)->value);
			while(*str != '\0')
			{
				switch(*str)
				{
					case '\\':
						fprintf(stream, "\\\\");
						break;
					case '\n':
						fprintf(stream, "\\n");
						break;
					case '\t':
						fprintf(stream, "\\t");
						break;
					case '"':
						fprintf(stream, "\\\"");
						break;
					default:
						putchar(*str);
				}
				str++;
			}
			putchar('"');
			break;
		case EMPTY_LIST:
			fprintf(stream, "()");
			break;
		case PAIR:
			putc('(', stream);
			write_pair(context, stream, (scheme_pair*)obj);
			putc(')', stream);
			break;
		case SYMBOL:
			fprintf(stream, "%s", ((scheme_symbol*) obj)->value);
			break;
		case PRIMITIVE_PROCEDURE:
		case COMPOUND_PROCEDURE:
			fprintf(stream, "#<procedure>");
			break;
		case ENVIRONMENT:
			fprintf(stream, "#<environment>");
			break;
		case INPUT_PORT:
			fprintf(stream, "(#<input port>)");
			break;
		case OUTPUT_PORT:
			fprintf(stream, "(#<output port>)");
			break;
		case EOF_OBJECT:
			fprintf(stream, "(#<eof>)");
			break;
		default:
			fprintf(stderr, "Unrecognized type: %d\n", obj->type);
			CRASH_MACRO;
			exit(1);
	}
}
