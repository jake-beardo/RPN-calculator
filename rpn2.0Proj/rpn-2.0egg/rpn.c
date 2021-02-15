/* rpn.c file */


/* libraries */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>
#include <errno.h>
#include "bigint.h"
#include "rational.h"
#include "rpn.h"




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


/* identifies if and what operator was provided by the user */ 
int isoperator(char *token)
{
  return (
         !strcmp(token, "+")
      || !strcmp(token, "-")
      || !strcmp(token, "x")
      || !strcmp(token, "/")
      );
}


/* this function not only checks if a fraction is provided as an arguments but also identifies any invalid tokens.
     */
int isfract(char*token, int *NUMnumerator, int *NUMdenominator)
{
  char *denominator;
  char numerator[128];
  int token_len = strlen(token), digit = 0, division_sign = 0, invalid_char = 0;

  //For loop searches for what is in the provided arguments and how many of each characters are in argument
  // data is then used to see if token is valid
  for (int i = 0; i < token_len; i++)
  {
    if (isdigit(token[i]) != 0)
    {
      digit++;
    }
    else if(token[i] == '/')
    {
      division_sign++;
    }
    else if(!isdigit(token[i] && token[i] != '/'))
    {
      invalid_char++;
    }
  }
  if(division_sign != 1 && digit != (token_len + 1))
  {
    fprintf(stderr, "Invalid token \"%s\"\n", token);
    return 0;
  }
  
  strcpy (numerator, token);
  strtok_r (numerator, "/", &denominator);
  *NUMnumerator = strtol(numerator, NULL, 10);
  *NUMdenominator = strtol(denominator, NULL, 10);
  if (*NUMdenominator <= 0)
  {
    fprintf(stderr, "Invalid token \"%s\"\n", token);
    return 0; 
  }
  fract_main(*NUMnumerator, *NUMdenominator);
  return 1;

}

/*
  Handle operator function calls bigint operator functions with necessary operands to give a result which is then
  pushed back onto the stack

  Also produces errors if operations are not possible.
*/
int handle_operator(char *operator, bigint operand1, bigint operand2, bigint* result)
{
  bigint zero = bigint_from_string_d("0");

  if(!strcmp(operator, "+")) {
    *result = bigint_add(operand1, operand2);
    return 0;
  }
  else if(!strcmp(operator, "-")) {

    *result = bigint_sub(operand1, operand2);

    return 0;
  }

  else if(!strcmp(operator, "x")) {

    *result = bigint_mul(operand1, operand2);

    return 0;
  }
  else if(!strcmp(operator, "/")) {
    if(bigint_eq(operand2, zero))
    {
      fprintf(stderr, "Zero division in ");
      bigint_print_d(operand1, stderr); 
      fprintf(stderr, " / ");
      bigint_print_d(operand2, stderr); 
      fprintf(stderr, "\n");
      return 1;
    }
    else
    {
      *result = bigint_div(operand1, operand2);
      return 0;
    }
  }
  else{
    return 1;
  }
}


/* 
 rpnmain function simply holds necessary varables for program , such as stack and stacksize.
 It then cycles through arguments provided by the user to identify whether the argument is a operand, operator or fraction
 */ 
int rpnmain(int argc, char *argv[])
{
  int max_stacksize = 10;
  bigint stack[max_stacksize];
  int stacksize = 0;

  char *token;
  bigint result;
  int NUMnumerator, NUMdenominator;


  for(int token_number=0; token_number<argc; token_number++)
  {
    token = argv[token_number];
    // Operand: positive and negative integers
    if(isoperand(token))
    {
      if(stacksize == max_stacksize) {
        fprintf(stderr, "Stack overflow at \"%s\"\n", token);
        return 1;
      }
      else
      {
        bigint operand = bigint_from_string_d(argv[token_number]);
        stack[stacksize] = operand;
        stacksize++;
      }
    }
    
    /* Operator: + - / x */
    else if(isoperator(token))
    {
      /*  Every operator is binary ...  */
      if(stacksize < 2) {
        fprintf(stderr, "Stack underflow at \"%s\"\n", token);
        return 1;
      }
      bigint operand1 = stack[stacksize-2];
      bigint operand2 = stack[stacksize-1];
      if(handle_operator(token, operand1, operand2, &result))
      {
        return 1;
      }
      stack[stacksize-2] = result;
      stacksize--;
    }
    else if(!isfract(token, &NUMnumerator, &NUMdenominator))
    {
      return 1;
    }

    else
    {
      //printf("%d/%d\n", NUMnumerator, NUMdenominator);
      //fprintf(stderr, "Invalid token \"%s\"\n", token);
      return 0;
    }
  }


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