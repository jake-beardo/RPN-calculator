/* rpn.c headerfile */

/* All libraies needed for program */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>
#include <errno.h>


/* Function Declerations */


/* The bits and pieces assembled */

/* checks if token is valid number */
int isoperand(char *token);

/* checks if token is add, subract, multiply or divide */
int isoperator(char *token);

/* checks if input is a fraction and if so is sent to rational.c functions */
int isfract(char*token, int *numerator1, int *denominator1);

/* givies instructions of what operations need to occur depending on arguments input
 and gives back a result to the rpnmain function */
int handle_operator(char *operator, bigint operand1, bigint operand2, bigint* result);

/* main calc function to identify operands and operators */
int rpnmain(int argc, char *argv[]);