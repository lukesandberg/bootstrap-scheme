#include <read.h>
#include <ctype.h>
#include <stdlib.h>

static char is_delimiter(int c)
{
	return isspace(c)
	|| c == EOF
	|| c == '('
	|| c == ')'
	|| c == '"'
	|| c == ';';
}
static char is_initial(int c)
{
	return isalpha(c) || c == '*' || c == '/' || c == '>' ||
	c == '<' || c == '=' || c == '?' || c == '!';
}

static int peek(FILE* in)
{
	int c = getc(in);
	ungetc(c, in);
	return c;
}

static void eat_whitespace(FILE* in)
{
	int c;
	while((c = getc(in)) != EOF)
	{
		if(isspace(c))
		{		
			continue;
		}
		else if(c == ';')//a comment, skip till the end of the line
		{
			while(((c = getc(in)) != EOF) && (c != '\n'));
			continue;
		}
		ungetc(c, in);
		break;
	}
}

static void peek_expected_delimiter(FILE* in)
{
	if(!is_delimiter(peek(in)))
	{
		fprintf(stderr, "expected a delimiter\n");
		exit(1);
	}
}

static void eat_expected_string(FILE* in, char* str)
{
	while(*str != '\0')
	{
		int c = getc(in);
		if(c != *str)
		{
			fprintf(stderr, "unexpected character literal %c, expected '%c\n", c, *str);
			exit(1);
		}
		str++;
	}
}

static scheme_object* read_character(vm* context, FILE* in)
{
	int c = getc(in);
	switch(c)
	{
		case EOF:
			fprintf(stderr, "incomplete character literal\n");
			exit(1);
		case 's':
			if(peek(in) == 'p')
			{
				eat_expected_string(in, "pace");
				peek_expected_delimiter(in);
				return make_character(context, ' ');
			}
			break;
		case 'n':
			if(peek(in) == 'e')
			{
				eat_expected_string(in, "ewline");
				peek_expected_delimiter(in);
				return make_character(context, '\n');
			}
			break;
		case 't':
			if(peek(in) == 'a')
			{
				eat_expected_string(in, "ab");
				peek_expected_delimiter(in);
				return make_character(context, '\t');
			}
			break;
	}
	peek_expected_delimiter(in);
	return make_character(context, c);
}

static scheme_object* read_string(vm* context, FILE* in)
{
	string_t* str = string_create();
	
	//we read until the first unescaped double quote character
	int c;
	while((c = getc(in)) != '"')
	{
		if(c == '\\')
		{
			//this is an escaped character
			c = getc(in);
			if(c == 'n')
			{
				c = '\n';
			}
			else if(c == 't')
			{
				c = '\t';
			}
		}
		if(c == EOF)
		{
			fprintf(stderr, "Unexpected termination of a string literal");
			exit(1);
		}
		string_append_char(str, c);
	}
	return make_string(context, str);
}

static scheme_object* read_symbol(vm* context, FILE* in)
{
	string_t* str = string_create();
	int c = getc(in);
	while(is_initial(c) || isdigit(c) || c == '+' || c == '-')
	{
		string_append_char(str, c);
		c = getc(in);
	}
	if(!is_delimiter(c))
	{
		fprintf(stderr, "symbol not folowed by delimiter\n");
		exit(1);
	}
	ungetc(c, in);
	scheme_object* rval = make_symbol(context, string_cstring(str));
	string_free(str);
	return rval;
}

static scheme_object* read_pair(vm* context, FILE* in)
{
	int c;
	scheme_object* car = NULL;
	scheme_object* cdr = NULL;
	eat_whitespace(in);
	c = getc(in);
	
	if(c ==')')
	{
		return the_empty_list;
	}

	gc_push_root((void**) &car);
	gc_push_root((void**) &cdr);
	
	ungetc(c, in);
	
	car = read(context, in);
	eat_whitespace(in);
	
	c = getc(in);
	if(c == '.')
	{
		if(!is_delimiter(peek(in)))
		{
			fprintf(stderr, "Expected a delimiter after '.'...\n");
			exit(1);
		}		
		cdr = read(context, in);
		
		eat_whitespace(in);

		c = getc(in);
		if(c != ')')
		{
			fprintf(stderr, "Expected a ')'\n");
			exit(1);
		}
	}
	else
	{
		ungetc(c, in);
		cdr = read_pair(context, in);
	}
	gc_pop_root();
	gc_pop_root();
	return cons(context, car, cdr);
}



scheme_object* read(vm* context, FILE* in)
{
	int c;
	eat_whitespace(in);
	c = getc(in);
	
	if(isdigit(c) || (c == '-' && (isdigit(peek(in)))))
	{
		short sign = 1;//positive
		long num = 0;
		//read a number
		if(c == '-')
		{
			sign = -1;
		}
		else
		{
			ungetc(c, in);
		}
		while(isdigit(c = getc(in)))
		{
			num = num *10 + (c -'0');
		}
		num *= sign;
		if(is_delimiter(c))
		{
			ungetc(c, in);
			return make_number(context, num);
		}
		
		//its not followed by a delimiter
		fprintf(stderr, "Number not followed by a delimiter!\n");
		exit(1);
	}
	else if(c == '#')
	{//character or boolean
		c = getc(in);

		switch(c)
		{
			case 't':
				return (scheme_object*) true;
			case 'f':
				return (scheme_object*) false;
			case '\\':
				return read_character(context, in);
			default:
				fprintf(stderr, "unknown boolean or character literal\n");
				exit(1);
		}
	}
	else if(c =='"')
	{
		return read_string(context, in);
	}
	else if(c == '(')
	{
		return read_pair(context, in);
	}
	else if(is_initial(c) || ((c == '+' || c == '-') && is_delimiter(peek(in))))
	{
		ungetc(c, in);
		return read_symbol(context, in);
	}
	else if(c == '\'')
	{
		return cons(context, (scheme_object*) quote_symbol, cons(context, read(context, in), the_empty_list));
	}
	else if(c == EOF)
	{
		return NULL;
	}
	else
	{
		fprintf(stderr, "Unexpected input: '%c'\n", c);
		exit(1);
	}
	fprintf(stderr, "read: illegal state\n");
	exit(1);
}
