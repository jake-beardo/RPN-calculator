/*
Our main function is near the top of the code underneath the include statements, stack array definitions and the function declarations and underneath our main function are all our other functions.
The main function contains the main body of our code and handles when different number of arguments are passed and different scenarios for different arguments.
The empty stack function returns 1 when there are no items left on the stack.
The full stack function returns 1 when the stack is full and no more items can be added to the stack.
The pop function removes and returns the item at the top of the stack.
The push function adds an item to the top of the stack.
The operator functions (add, subtract, multiply and divide) perform operations on the top two items of the stack and pushes the result back onto the stack. Error messages are printed if the operation cannot be performed.
The integer_overflow_in_operation function will return 1 if the output of an operation is outside the boundaries of INT_MAX and INT_MIN and produce an error message.
The integer_overflow_in_token will return 1 if the argument integer is outside the boundaries of INT_MAX and INT_MIN and produce an error message.
The invalid_token function will return 1 if an argument is neither and integer or an operator and produce an error message.
*/

#include <stdio.h>
#include <limits.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "bigint.h"
#include "rpn.h"

int stack[10];
int top = -1;

// funciton declarations
int string_to_int(char argument);
int empty_stack();
int full_stack();
int pop();
int push(int item);
int add();
int subtract();
int multiply();
int divide();
int integer_overflow_in_operation(char operator, int operand_1, int operand_2);
int integer_overflow_in_token(char *argument);
int invalid_token(char *argument);

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
		/*
		the for loop, loops through the input arguments
		if the first character of an argument is an operator then the operation is performed
		if the operation cannot be performed, an error has occurred and the program returns 1
		if an argument is invalid an error has occurred and the program returns 1
		if the argument is an integer, the argument is pushed on to the stack array
		if the integer cannot be pushed on to the stack an error has occurred and the program returns 1
		*/
		for(int i = 1; i < argc; i++)
		{
  			if(argv[i][0] == '+')
  			{
  				if (!add())
				{
					return 1;
				}
  			}
  			//only performs subtraction when the minus sign is seen by itself
			else if(argv[i][0] == '-' && strlen(argv[i]) == 1)
  			{
  				if (!subtract())
				{
					return 1;
				}
  			}
  			else if(argv[i][0] == 'x')
  			{
  				if (!multiply())
				{
					return 1;
				}
  			}
  			else if(argv[i][0] == '/' )
  			{
  				if (!divide())
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
				int number = strtol(argv[i], NULL, 10);
				if(!push(number))
				{
					return 1;
				}
			}
  		}
	}
		
	while(!empty_stack())
	{
		/*	
		when there's more than 1 item left on stack, error message is printed, printing all the items left on stack and program returns 1
		when there's only one item left on stack, output is printed
		*/
		if(top > 0)
		{
			fprintf(stderr, "Tokens left on stack:\n");
			for(int i = 0; i <= top; i++)
			{
				fprintf(stderr, "stack[%d] = %d\n", i, stack[i]);
			}
			return 1;
		}
		
		else
		{
			printf("%d\n", pop());
		}
	}
	return 0;
}

int empty_stack()
{
	// when highest position of the stack is at -1, stack is empty
	if(top == -1)
	{
		return 1;
	}
	return 0;
}

int full_stack()
{
	// when highest position of the stack is greater or equal to 9, stack is full
	int maxsize = 9;
	if(top >= maxsize)
	{
		return 1;
	}
	return 0;
}

int pop()
{	
	/*
	as long as the stack is not empty the function returns the item at the highest position on the stack
	the highest position of the stack is reduced by one
	*/
	if(!empty_stack())
	{
		int item = stack[top];
		top = top - 1;
		return item;
	}
	return 0;
}

int push(int item)
{
	/*
	an integer is passed in as a parameter to the function
	as long stack is not full, the integer is added to the top of the stack
	if stack is full an error message is printed
	*/
	if(!full_stack())
	{
		top = top + 1;
		stack[top] = item;
		return 1;
	}
	else
	{
		fprintf(stderr, "Stack overflow at \"%d\"\n", item);
		return 0;
	}
}	

int add()
{
	/* 
	if there's only one item left on the stack, there is a stack underflow and an error message is printed
	if there's an overflow in operation an error message is printed, otherwise operation is performed and output pushed back onto the stack
	*/
	if (top < 1)
	{
		fprintf(stderr, "Stack underflow at \"+\"\n");
		return 0;
	}
	else
	{
		int a = pop(), b = pop();
		if (integer_overflow_in_operation('+', b, a))
		{
			return 0;
		}
		push(b + a);
		return 1;
	}
}
	
int subtract()
{
	/* 
	if there's only one item left on the stack, there is a stack underflow and an error message is printed
	if there's an overflow in operation an error message is printed, otherwise operation is performed and output pushed back onto the stack
	*/
	if (top < 1)
	{
		fprintf(stderr, "Stack underflow at \"-\"\n");
		return 0;	
	}
	else
	{
		int a = pop(), b = pop();
		if(integer_overflow_in_operation('-', b, a))
		{
			return 0;
		}
		push(b - a);
		return 1;
	}
	
}
int multiply()
{
	/* 
	if there's only one item left on the stack, there is a stack underflow and an error message is printed
	if there's an overflow in operation an error message is printed, otherwise operation is performed and output pushed back onto the stack
	*/
	if (top < 1)
	{
		fprintf(stderr, "Stack underflow at \"x\"\n");
		return 0;
	}
	else
	{
		int a = pop(), b = pop();
		if(integer_overflow_in_operation('x', b, a))
		{
			return 0;
		}
		push(b * a);
		return 1;
	}
}	

int divide()
{
	/* 
	if there's only one item left on the stack, there is a stack underflow and an error message is printed
	if there's an overflow in operation an error message is printed and function is returned unsucessful
	if there is division by 0 or division does not return an integer, an error message is printed, otherwise operation is performed and output pushed back onto the stack
	*/
	if (top < 1)
	{
		fprintf(stderr, "Stack underflow at \"/\"\n");
		return 0;
	}
	else
	{
		int a = pop(), b = pop();
		if(integer_overflow_in_operation('/', b, a))
		{
			return 0;
		}
		else if(a == 0)
		{
			fprintf(stderr, "Zero division in %d / %d\n", b, a);
			return 0;
		}
		else if(b % a != 0)
		{
			fprintf(stderr, "%d is not divisible by %d\n", b, a);
			return 0;
		}
		push(b / a);
		return 1;
	}	
}

int integer_overflow_in_token(char *argument)
{
	/*
	string is converted to long integer
	if long integer is greater than INT_MAX  or less than INT_MIN an error message for integer overflow is printed
	otherwise the integer is printed
	*/
	long long_val = strtol(argument, NULL, 10);
	if ((long_val == LONG_MIN && errno == ERANGE) || (long_val == LONG_MAX && errno == ERANGE) || long_val < INT_MIN ||	long_val > INT_MAX) 
	{
		fprintf(stderr, "Integer overflow at token \"%s\"\n", argument);
		return 1;
	}
	else
	{
		printf("%ld\n", long_val);
		return 0;
	}
}

int invalid_token(char *argument)
{
    /*
	the for loop, loops through characters in each argument and counts the number of alphabets, minus signs and other characters there are
	if the number is above the specified limit an invalid token error message is printed
	*/
	int argument_length = strlen(argument), alphabet = 0, negative_sign = 0, character = 0; 
	
    for (int i = 0; i < argument_length; i++) 
    { 
    // check for alphabets 
		if (isalpha(argument[i]) != 0) 
		{
			alphabet++; 
		}
		else if( argument[i] == '-')
		{
			negative_sign++;
		}
		else if(isdigit(argument[i]) == 0 && isalpha(argument[i]) == 0 && argument[i] != '-')
		{
			character++;
		}
    }
	
    if( alphabet > 0 || negative_sign > 1 || character > 0)
    {
        fprintf(stderr, "Invalid token \"%s\"\n", argument);
        return 1;
    }
	return 0;
}

int integer_overflow_in_operation(char operator, int operand_1, int operand_2)
/*
for each specific operator, there is a check for whether there will be an integer overflow as a result of the operation
if there is an integer overflow, an error message is printed
*/
{
	if(operator == '/')
	{
		if(operand_1 < 0 && operand_2 < 0 && operand_1 < INT_MAX * operand_2)
		{
			fprintf(stderr, "Integer overflow in %d / %d\n", operand_1, operand_2);
			return 1;
		}
	}
	else if (operator == '+')
	{
		if((operand_1 > 0 && operand_2 > 0 && operand_1 > INT_MAX - operand_2) || (operand_1 < 0 && operand_2 < 0 && operand_1 < INT_MIN - operand_2))
		{
			fprintf(stderr, "Integer overflow in %d + %d\n", operand_1, operand_2);
			return 1;
		}
	}
	else if(operator == 'x')
	{
		if((operand_1 > 0 && operand_2 > 0 && operand_1 > INT_MAX / operand_2) || (operand_1 > 0 && operand_2 < 0 && operand_1 < INT_MIN / operand_2)
			|| (operand_1 < 0 && operand_2 > 0 && operand_1 < INT_MIN / operand_2) || (operand_1 < 0 && operand_2 < 0 && operand_1 < INT_MAX / operand_2))
		{
			fprintf(stderr, "Integer overflow in %d x %d\n", operand_1, operand_2);
			return 1;
		}
	}
	else if(operator == '-')
	{
		if((operand_1 > 0 && operand_2 < 0 && operand_1 > INT_MAX - -operand_2) || (operand_1 < 0 && operand_2 > 0 && operand_1 < INT_MIN + operand_2))
		{
			fprintf(stderr, "Integer overflow in %d - %d\n", operand_1, operand_2);
			return 1;
		}
	}
	return 0;
}