/* rpnmain.c file */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>
#include <errno.h>
#include "bigint.h"
#include "rational.h"

/* Define this in one place so it isn't duplicated */
const char USAGE_STRING[] =\
    "./rpn.exe --usage\n"
    "./rpn.exe --version\n" /* Strings auto-concatenated (no semicolon) */
    "./rpn.exe TOKENS...\n";

const char VERSION_STRING[] = "2.0";

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
