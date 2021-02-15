/* 
Function definitions
creates a struct to form fractional number type
 */

struct fractional_number 
{
	int num;
	int den;
} typedef fraction;

/* fract_main converts input numerator and denominator into fraction struct form */
int fract_main(int numerator, int denominator);

/* simplifies fractions into their lowest form */
int simplified_fraction(fraction x);