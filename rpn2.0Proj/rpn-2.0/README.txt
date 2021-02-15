Author: Jake Beard
Date: 23/02/2019
Description: This program is a Reverse Polish Notation (RPN) Calculator which an add, subtract, multiply or divide.
To use:
Enter “./rpn.exe …..” then provide arguments in the format below:
“./rpn.exe 1 2 +”     . . . To give the result 1 + 2 = 3

./rpn.exe 1 2 3 + + 6 / . . .  

To give the result  (1 + 2 + 3) / 6 = 1

Things to note:
- Programme can’t take more than 10 operands
- Calculator integer min and max are -2147483648 and +2147483647 respectively.
- use the symbols add(+), subtract(-), multiply(x) and divide(/).


To check version of calculator enter argument: —version

To check usage enter argument: —usage

Changelog:

28/04/2019 - Makefile which can be easily read to see what c and h files are involved in the program. 

25/04/2019 - Made a simplify fractions function within rational.c to essentially simplify fractions if possible

25/04/2019 - created fractional numbers struct to begin using fractions and ways of handling them differently to regular integers. Also made a function to identify rational numbers when they’re input as an argument. 

21/04/2019 - Changed the BIGINTS MAX digits variable to 255 so big ints can handle much larger numbers.

19/04/2019 - Created handle operations function to multiple divide subtract and add when needed using the bigints functions given to us.

19/04/2019 - Changed some variable names to bigints such as stack, results and operands 

19/04/2019 - Removed all unnecessary functions from the program such as integer overflow and overflow in operations since there will no longer be overflow

VERSION 2.0

13/03/2019 - If more than 2 arguments are provided and one of them is an invalid token the program produces an error message.

12/03/2019 - Added output error if there are 2 or more numbers left on the stack.

06/03/2019 - Removed global variables such as maxsize 

04/03/2019 - Integer overflow in operation function: checks for all possible overflow conditions for each operator.

03/03/2019 - altered code so exit(1) code now outputs a return value to make code run more elegantly so it can be changed in future to continue running .

03/03/2019 - when running operator functions to check if the function fails we changed the if statements to if !operator return 1 to end the main loop and produce error message.

03/03/2019 - created separate functions for integer overflow, integer overflow during operation and invalid token instead of having these checks in the main loop.

01/03/2019 - For each possible operation if there are too many operators and not enough operands error messages are given.

01/03/2019 - Changed order of if statements in division function to check for errors before it does an operation .  

01/03/2019 - With the addition of limits library, we created a function to check for integer overflow.

28/02/2019 - Created invalid token function, which uses isalpha and isdigit to check 


26/02/2019 - imported relevant libraries to stop warnings. 

24/02/2019 - when cycling through arguments if program encounters a ‘-‘ sign it automatically goes to subtract function so to prevent this from occurring when the program encounters negative numbers we changed the if statement so it conducts subtract only when the string length is 1 

23/02/2019 - added pointers for -—usage, —-version and argv[1]. This then allowed the use of the strcmp function to compare strings that were given as arguments.

22/02/2019 - Added empty and full stack functions to produce errors when too few or too many elements on the stack.

21/02/2019 - created our stack of 10 elements and functions pop and push to take elements off and add elements onto the stack. Then we were able to print these elements to check which order they were going onto and coming off the stack.
