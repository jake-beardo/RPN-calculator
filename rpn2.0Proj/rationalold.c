#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "bigint.h"
#include "rational.h"

int fract_main(numerator, denominator)
{
	fraction frac_1 = {numerator, denominator};
	fraction frac_2 = {0, 0};
	fractional_addition(frac_1, frac_2);
	fractional_subtraction(frac_1, frac_2);
	fractional_multiplication(frac_1, frac_2);
	fractional_division(frac_1, frac_2);
	return 0;
}

int fractional_addition(fraction x, fraction y)
{
	fraction normalised_x = {x.num * y.den, x.den * y.den};
	fraction normalised_y = {y.num * x.den, y.den * x.den};
	fraction added_fraction = {normalised_x.num + normalised_y.num, normalised_y.den};
	simplified_fraction(added_fraction.num, added_fraction.den);
	return 1;
}

int fractional_subtraction(fraction x, fraction y)
{
	fraction normalised_x = {x.num * y.den, x.den * y.den};
	fraction normalised_y = {y.num * x.den, y.den * x.den};
	fraction subtracted_fraction = {normalised_x.num - normalised_y.num, normalised_y.den};
	simplified_fraction(subtracted_fraction.num, subtracted_fraction.den);
	return 1;
}

int fractional_multiplication(fraction x, fraction y)
{
	fraction multiplied_fraction = {x.num * y.num, x.den * y.den};
	simplified_fraction(multiplied_fraction.num, multiplied_fraction.den);
	return 1;
}

int fractional_division(fraction x, fraction y)
{
	fraction divided_fraction = {x.num * y.den, x.den * y.num};
	simplified_fraction(divided_fraction.num, divided_fraction.den);
	return 1;
}

int simplified_fraction(int numerator, int denominator)
{
	int original_denominator = denominator;
	int original_numerator = numerator;
	int remainder;
	int number;
	while(remainder != 0) 
	{
		remainder = denominator % numerator;
		if(remainder == 0)
		{
			number = numerator; 
		}
		else
		{
			denominator = numerator;
			numerator = remainder;
		}
	}
	fraction simple_frac = {original_numerator / number, original_denominator / number};
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
		