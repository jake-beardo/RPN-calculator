
#include <stdio.h>
#include <limits.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "bigint.h"

int main(int argc, char *argv[])
{
	char *version = "--version", *usage = "--usage", *argv1 = argv[1];
	
	if(argc == 2)
	{
		/*
		when one argument is inputted by the user, if the argument is '--version' or '--usage' a specific output is printed.
		if the argument is invalid or the token is an integer overflow an error message is printed and program returns 1, otherwise the argument is printed as the output.
		*/
		if (strcmp(version, argv1) == 0)
		{
			printf("1.0\n");
		}
		else if(strcmp(usage, argv1) == 0)
		{
			printf("./rpn.exe --usage\n");
			printf("./rpn.exe --version\n");
			printf("./rpn.exe TOKENS...\n");
		}
		else
		{
			if(invalid_token(argv1))
			{
				return 1;
			}
        	else if(integer_overflow_in_token(argv1))
			{
				return 1;
			}
  		}
  	}

	else if(argc == 1)
	{
		//when no arguments are inputted by the user, an error message is printed
		fprintf(stderr, "./rpn.exe --usage\n");
		fprintf(stderr, "./rpn.exe --version\n");
		fprintf(stderr,"./rpn.exe TOKENS...\n");
		return 1;
	}
	
	else
	{
		bigint number = bigint_from_string_d(argv[1]);
		bigint_print_d(number, stdout);
		printf("\n");

		for(int i = 1; i < argc; i++)
		{
  			if(argv[i][0] == '+')
  			{
  				if (!bigint_add(bigint bi1, bigint bi2))
				{
					return 1;
				}
  			}
  			//only performs subtraction when the minus sign is seen by itself
			else if(argv[i][0] == '-' && strlen(argv[i]) == 1)
  			{
  				if (!bigint_sub(bigint bi1, bigint bi2))
				{
					return 1;
				}
  			}
  			else if(argv[i][0] == 'x')
  			{
  				if (!bigint_mul(bigint bi1, bigint bi2))
				{
					return 1;
				}
  			}
  			else if(argv[i][0] == '/' )
  			{
  				if (!bigint_div(bigint bin, bigint bid))
  				{
					return 1;
  				}
			}
  			else if(invalid_token(argv[i]))
  			{
  				return 1;
  			}
  			
  			else 
  			{
				bigint number = bigint_from_string_d(argv[i]);
				if(!push(number))
				{
					return 1;
				}
			}
		}
	}
  		/*

	    bigint bi1 = bigint_from_string_d(argv[1]);
	    char* op = argv[2];
	    bigint bi2 = bigint_from_string_d(argv[3]);

	    bigint result;
	    if(!strcmp(op, "+")) {
	        result = bigint_add(bi1, bi2);
	    }
	    else if(!strcmp(op, "-")) {
	        result = bigint_sub(bi1, bi2);
	    }
	    else if(!strcmp(op, "*")) {
	        result = bigint_mul(bi1, bi2);
	    }
	    else if(!strcmp(op, "//")) {
	        result = bigint_div(bi1, bi2);
	    }
	    else {
	        fprintf(stderr, "Unrecognised operator \"%s\"\n", op);
	        return 1;
	    }

	    bigint_print_d(bi1, stdout);
	    printf(" %s ", op);
	    bigint_print_d(bi2, stdout);
	    printf(" == ");
	    bigint_print_d(result, stdout);
	    printf("\n");
	    return 0;










	        /* Is a number? 
    if(isoperand(token))
    {
      if(stacksize == max_stacksize) {
        fprintf(stderr, "Stack overflow at \"%s\"\n", token);
        return 1;
      }
      int operand;
      if(!bigint_from_string_d(argv[token_number])) {
        fprintf(stderr, "Integer overflow at token \"%s\"\n", token);
        return 1;
      }
      stack[stacksize] = operand;
      stacksize++;
    }
	  */
	}
}
