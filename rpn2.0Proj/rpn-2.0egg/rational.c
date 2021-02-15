#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "rational.h"

/* takes input and converts to fraction type where it can be handeled */

int fract_main(numerator, denominator)
{
	fraction frac_1 = {numerator, denominator};
	simplified_fraction(frac_1);

	return 0;
}

/* checks if there is a remainder from the modulus of the numerator and demoninator of the fraction
if there is a remainder then fraction is already in its smallest form and the function will leave the fraction unchanged, however if not then the fraction can 
be simplified. The function will then divide the fractions numerator and denominator by it's numerator to simplify the fraction.
Once the fraction is simplfied the function prints the output. */

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
		