#define _GNU_SOURCE
#include <stdio.h>

#include <model/model.h>
#include <read.h>
#include <eval.h>
#include <write.h>

/*********************************REPL**************************************/
#include <string.h>

int main(int argc, char** argv)
{
	vm* context =  vm_get();
	if(argc > 1)
	{
		for(int i = 1; i < argc; i++)
		{
			printf("%s\n", argv[i]);
			FILE* strstrm = fmemopen(argv[i], strlen(argv[i]), "r");
			write(context, stdout, eval(context, read(context, strstrm)));
			printf("\n");
			fclose(strstrm);
		}
	}
	else
	{
		printf("Welcome to Bootstrap Scheme. Use ctrl-c to exit.\n");
		while(1)
		{
			printf("> ");
			write(context, stdout, eval(context, read(context, stdin)));
			printf("\n");
		}
	}


}

