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
#include <ctype.h>
#include "bigint.h"

/* Define this in one place so it isn't duplicated */
const char USAGE_STRING[] =\
    "./rpn.exe --usage\n"
    "./rpn.exe --version\n" /* Strings auto-concatenated (no semicolon) */
    "./rpn.exe TOKENS...\n";

const char VERSION_STRING[] = "2.0";


/* The main workhorse */
int rpnmain(int argc, char *argv[]);

/* The bits and pieces assembled */
int string_to_int(char *str, int *result);
int isoperand(char *token);
int isoperator(char *token);
int invalid_tok(char *argument);
bigint handle_operator(char *operator, bigint operand1, bigint operand2, bigint* result);



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
  bigint stack[max_stacksize];
  int stacksize = 0;
  char *token;

  for(int token_number=0; token_number<argc; token_number++)
  {
    token = argv[token_number];
    
    /* Operator: + - / x */
    if(isoperator(token))
    {
      /*  Every operator is binary ...  */
      if(stacksize < 2) {
        fprintf(stderr, "Stack underflow at \"%s\"\n", token);
        return 1;
      }
      bigint operand1 = stack[stacksize-2];
      bigint operand2 = stack[stacksize-1];
      bigint result = handle_operator(token, operand1, operand2);
      stack[stacksize-2] = result;
      stacksize--;
    }
    else if(invalid_tok(token))
    {
      return 1;
    }

    else
    {
      if(stacksize == max_stacksize) {
        fprintf(stderr, "Stack overflow at \"%s\"\n", token);
        return 1;
      }
      bigint operand = bigint_from_string_d(argv[token_number]);
      bigint_print_d(operand, stdout);
      stack[stacksize] = operand;
      stacksize++;
    }
  }
    /* Not a number or an operator */

          /* Is a number? */

  /* At end stack should have exactly one item */
  if(stacksize > 1) {
    fprintf(stderr, "Tokens left on stack:\n");
    for(int i=0; i<stacksize; i++) {
      bigint_print_d(stack[i], stdout);
      printf("\n");
    }
    return 1;
  }
  else if(stacksize == 0) {
    fprintf(stderr, "No input.\n");
    return 1;
  }
  else {
    // Success!
    bigint_print_d(stack[0], stdout);
    printf("\n");
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


int invalid_tok(char *argument)
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

/*
 * Parse decimal string str into result.
 *
 * Returns 0 in the case of overflow and 1 for success.
 * Doesn't check that the string is valid decimal.
 */

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

bigint handle_operator(char *operator, bigint operand1, bigint operand2)
{
  if(!strcmp(operator, "+")) {
    return bigint_add(operand1, operand2);
  }
  else if(!strcmp(operator, "-")) {

    return bigint_sub(operand1, operand2);  
  }

  else if(!strcmp(operator, "x")) {
    return bigint_mul(operand1, operand2);
  }
  else if(!strcmp(operator, "/")) {
    return bigint_div(operand1, operand2);
  }
  else { // Execution should never reach here...
    fprintf(stderr, "Runtime error... in handle_operator\n");
  }
}

