/*
 * File: rpnmain.c
 * Author: Oscar Benjamin
 * Created: 22nd March 2019
 *
 * This is a single file C program making rpn.exe which is a CLI for
 * evaluating expressions in reverse polish notation (RPN).
 *
 * Example:
 *
 *     $ ./rpn.exe 1 2 +
 *     3
 *
 * See the README for more.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>

/* Define this in one place so it isn't duplicated */
const char USAGE_STRING[] =\
    "./rpn.exe --usage\n"
    "./rpn.exe --version\n" /* Strings auto-concatenated (no semicolon) */
    "./rpn.exe TOKENS...\n";

const char VERSION_STRING[] = "1.0";


/* The main workhorse */
int rpnmain(int argc, char *argv[]);

/* The bits and pieces assembled */
int string_to_int(char *str, int *result);
int isoperand(char *token);
int isoperator(char *token);
int handle_operator(char *operator, int operand1, int operand2, int* result);
int handle_add(int operand1, int operand2, int* result);
int handle_sub(int operand1, int operand2, int* result);
int handle_mul(int operand1, int operand2, int* result);
int handle_div(int operand1, int operand2, int* result);


/*****************************************************************************
 *    The main function for the program.
 *
 *    This doesn't do much. It handles the cases where we don't actually
 *    calculate anything. The cases where we do are dispatched to rpnmain
 *    which can then get on with it without being distracted.
*****************************************************************************/

int main(int argc, char *argv[])
{
  /*     $ ./rpn.exe --version      */
  if(argc == 2 && !strcmp(argv[1], "--version")) {
    printf("%s\n", VERSION_STRING);
    return 0;
  }
  /*     $ ./rpn.exe --usage       */
  else if(argc == 2 && !strcmp(argv[1], "--usage")) {
    fputs(USAGE_STRING, stdout);
    return 0;
  }
  /*     $ ./rpn.exe               */
  else if(argc == 1) {
    fputs(USAGE_STRING, stderr);
    return 1;
  }
  /*     $ ./rpn.exe TOKEN1 TOKEN2 ...    */
  return rpnmain(argc-1, argv+1);
}


/*****************************************************************************
 *    The main RPN evaluation algorithm.
 *
 *    Reference:
 *    https://en.wikipedia.org/wiki/Reverse_Polish_notation#Postfix_evaluation_algorithm
*****************************************************************************/

int rpnmain(int argc, char *argv[])
{
  int max_stacksize = 10;
  int stack[max_stacksize];
  int stacksize = 0;
  char *token;

  for(int token_number=0; token_number<argc; token_number++)
  {
    token = argv[token_number];

    /* Is a number? */
    if(isoperand(token))
    {
      if(stacksize == max_stacksize) {
        fprintf(stderr, "Stack overflow at \"%s\"\n", token);
        return 1;
      }
      int operand;
      if(!string_to_int(token, &operand)) {
        fprintf(stderr, "Integer overflow at token \"%s\"\n", token);
        return 1;
      }
      stack[stacksize] = operand;
      stacksize++;
    }
    /* Operator: + - / x */
    else if(isoperator(token))
    {
      /*  Every operator is binary ...  */
      if(stacksize < 2) {
        fprintf(stderr, "Stack underflow at \"%s\"\n", token);
        return 1;
      }
      int operand1 = stack[stacksize-2];
      int operand2 = stack[stacksize-1];
      int result;
      if(!handle_operator(token, operand1, operand2, &result)) {
        return 1;
      }
      stack[stacksize-2] = result;
      stacksize--;
    }
    /* Not a number or an operator */
    else
    {
      fprintf(stderr, "Invalid token \"%s\"\n", token);
      return 1;
    }
  }

  /* At end stack should have exactly one item */
  if(stacksize > 1) {
    fprintf(stderr, "Tokens left on stack:\n");
    for(int i=0; i<stacksize; i++) {
      fprintf(stderr, "stack[%d] = %d\n", i, stack[i]);
    }
    return 1;
  }
  else if(stacksize == 0) {
    fprintf(stderr, "No input.\n");
    return 1;
  }
  else {
    // Success!
    printf("%d\n", stack[0]);
    return 0;
  }
}

/*****************************************************************************
 *      Parsing routines.
 *
 *      Probably an existing function for this but in C it's often easier just
 *      to write it yourself.
 * **************************************************************************/

/* An operand is a string of digits possibly starting with a single - */
int isoperand(char *token)
{
  if(*token == '-') {
    token++;
    if(!*token) {
      return 0;
    }
  }
  while(*token) {
    if(!('0' <= *token && *token <= '9')) {
      return 0;
    }
    token++;
  }
  return 1;
}

/*
 * Parse decimal string str into result.
 *
 * Returns 0 in the case of overflow and 1 for success.
 * Doesn't check that the string is valid decimal.
 */
int string_to_int(char *str, int *result)
{
  long long_val = strtol(str, NULL, 10);
  if ( (long_val == LONG_MIN && errno == ERANGE)
      || (long_val == LONG_MAX && errno == ERANGE)
      || long_val < INT_MIN
      || long_val > INT_MAX) {
    return 0;
  }
  *result = (int)long_val;
  return 1;
}

int isoperator(char *token)
{
  return (
         !strcmp(token, "+")
      || !strcmp(token, "-")
      || !strcmp(token, "x")
      || !strcmp(token, "/")
      );
}

/*****************************************************************************
 *      Arithmetic routines.
 *
 *      These would be trivial except that we check if the result overflows in
 *      32 bit arithmetic.
 * **************************************************************************/


/* Dispatch - function pointers would be a better solution here */
int handle_operator(char *operator, int operand1, int operand2, int* result)
{
  if(!strcmp(operator, "+")) {
    return handle_add(operand1, operand2, result);
  }
  else if(!strcmp(operator, "-")) {
    return handle_sub(operand1, operand2, result);
  }
  else if(!strcmp(operator, "x")) {
    return handle_mul(operand1, operand2, result);
  }
  else if(!strcmp(operator, "/")) {
    return handle_div(operand1, operand2, result);
  }
  else { // Execution should never reach here...
    fprintf(stderr, "Runtime error... in handle_operator\n");
    return 0;
  }
  return 1;
}

int handle_add(int operand1, int operand2, int* result)
{
  if((operand1 > 0 && operand2 > INT_MAX - operand1) ||
     (operand1 < 0 && operand2 < INT_MIN - operand1)) {
    fprintf(stderr, "Integer overflow in %d + %d\n", operand1, operand2);
    return 0;
  }

  *result = operand1 + operand2;
  return 1;
}

int handle_sub(int operand1, int operand2, int* result)
{
  if((operand1 >= 0 && operand1 - INT_MAX > operand2) ||
     (operand1 < 0 && operand1 - INT_MIN < operand2)) {
    fprintf(stderr, "Integer overflow in %d - %d\n", operand1, operand2);
    return 0;
  }

  *result = operand1 - operand2;
  return 1;
}

int handle_mul(int operand1, int operand2, int* result)
{
  if((operand1 > 0 && operand2 > 0 && operand1 > INT_MAX / operand2) ||
     (operand1 < 0 && operand2 < 0 && operand1 < INT_MAX / operand2) ||
     (operand1 > 0 && operand2 < 0 && operand2 < INT_MIN / operand1) ||
     (operand1 < 0 && operand2 > 0 && operand1 < INT_MIN / operand2)) {
    fprintf(stderr, "Integer overflow in %d x %d\n", operand1, operand2);
    return 0;
  }
  *result = operand1 * operand2;
  return 1;
}

int handle_div(int operand1, int operand2, int* result)
{
  /* The only possible way that division can overflow */
  if(operand2 == 0) {
    fprintf(stderr, "Zero division in %d / %d\n", operand1, operand2);
    return 0;
  }

  /* The only possible way that division can overflow */
  if(operand1 == INT_MIN && operand2 == -1) {
    fprintf(stderr, "Integer overflow in %d / %d\n", operand1, operand2);
    return 0;
  }

  /* Compute the actual result */
  *result = operand1 / operand2;

  /* Check for truncation in division */
  if(*result * operand2 != operand1) {
    fprintf(stderr, "%d is not divisible by %d\n", operand1, operand2);
    return 0;
  }

  /* Success! */
  return 1;
}
