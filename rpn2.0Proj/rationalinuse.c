#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "bigint.h"
#include "rational.h"

int fract_main(numerator, denominator)
{
	fraction frac_1 = {numerator, denominator};
	simplified_fraction(frac_1);

	return 0;
}


int simplified_fraction(fraction x)
{
	int denominator = x.den;
	int numerator = x.num;
	int remainder;
	int number;
	remainder = x.den % x.num;

	if(remainder == 0)
	{
		number = x.num; 
		number = abs(number);
		
	}
	else
	{
		number = 1;
	}
	fraction simple_frac = {numerator / number, denominator / number};
	if(simple_frac.den == 1)
	{
		printf("%d\n", simple_frac.num);
	}
	else if(simple_frac.num == 0) 
	{
		printf("%d\n", 0);
	}
	else
	{
		printf("%d/%d\n", simple_frac.num, simple_frac.den);
	}
	return 1;
}
		