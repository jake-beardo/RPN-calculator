{\rtf1\ansi\ansicpg1252\cocoartf2513
\cocoatextscaling0\cocoaplatform0{\fonttbl\f0\fswiss\fcharset0 Helvetica;}
{\colortbl;\red255\green255\blue255;}
{\*\expandedcolortbl;;}
\paperw11900\paperh16840\margl1440\margr1440\vieww10800\viewh8400\viewkind0
\pard\tx566\tx1133\tx1700\tx2267\tx2834\tx3401\tx3968\tx4535\tx5102\tx5669\tx6236\tx6803\pardirnatural\partightenfactor0

\f0\fs24 \cf0 Our main function is near the top of the code underneath the include statements, stack array definitions and the function declarations and underneath our main function are all our other functions.\
The main function contains the main body of our code and handles when different number of arguments are passed and different scenarios for different arguments.\
The empty stack function returns 1 when there are no items left on the stack.\
The full stack function returns 1 when the stack is full and no more items can be added to the stack.\
The pop function removes and returns the item at the top of the stack.\
The push function adds an item to the top of the stack.\
The operator functions (add, subtract, multiply and divide) perform operations on the top two items of the stack and pushes the result back onto the stack. Error messages are printed if the operation cannot be performed.\
The integer_overflow_in_operation function will return 1 if the output of an operation is outside the boundaries of INT_MAX and INT_MIN and produce an error message.\
The integer_overflow_in_token will return 1 if the argument integer is outside the boundaries of INT_MAX and INT_MIN and produce an error message.\
The invalid_token function will return 1 if an argument is neither and integer or an operator and produce an error message.}