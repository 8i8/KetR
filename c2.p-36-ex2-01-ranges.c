/*
 * Exercise 2-1. Write a program to determine the ranges of char, short, int,
 * and long variables, both signed and unsigned, by printing appropriate values
 * from standard headers and by direct computation. Harder if you compute them:
 * determine the ranges of the various floating-point types.
 */
#include <stdio.h>
#include <float.h>
#include <limits.h>

int main(void)
{
	unsigned short int 	a = 65535;
	short int 		b = 32767;
	unsigned int 		c = 4294967295;
	int 			d = 2147483647;
	unsigned long int	e = 18446744073709551615;
	long int		f = 9223372036854775807;
	float 			g = 10;
	float 			h = 3;
	double 			i = 10;
	double 			j = 3;
	long double 		k = 10;
	long double 		l = 3;

	printf("Type char :\n\n");
	printf("For char max UCHAR_MAX:~ %d\n", UCHAR_MAX);
	puts("");
	printf("For char max SCHAR_MAX:~ %d\n", SCHAR_MAX);
	printf("For char min SCHAR_MIN:~ %d\n", SCHAR_MIN);
	puts("");
	printf("For char max CHAR_MAX:~ %d\n", CHAR_MAX);
	printf("For char min CHAR_MIN:~ %d\n", CHAR_MIN);
	puts("");
	printf("Type short int :\n\n");
	printf("For unsigned short USHRT_MAX:~ %hu\n", USHRT_MAX);
	printf("%hd + 1 = %hd\n", a, a+1);
	puts("");
	printf("For short SHRT_MAX:~ %hd\n", SHRT_MAX);
	printf("For short SHRT_MIN:~ %hd\n", SHRT_MIN);
	printf("%hd + 1 = %hd\n", b, b+1);
	puts("");
	printf("Type int :\n\n");
	printf("For unsigned int UINT_MAX:~ %u\n", UINT_MAX);
	printf("%u + 1 = %u\n", c, c+1);
	puts("");
	printf("For int INT_MAX:~ %d\n", INT_MAX);
	printf("For int INT_MIN:~ %d\n", INT_MIN);
	printf("%d + 1 = %d\n", d, d+1);
	puts("");
	printf("Type long int :\n\n");
	printf("For unsigned long ULONG_MAX:~ %lu\n", ULONG_MAX);
	printf("%lu + 1 = %lu\n", e, e+1);
	puts("");
	printf("For int LONG_MAX:~ %ld\n", LONG_MAX);
	printf("For int LONG_MIN:~ %ld\n", LONG_MIN);
	printf("%ld + 1 = %ld\n", f, f+1);
	puts("");
	printf("Type floats :\n\n");
	printf("float MAX FLT_MAX:~ %f\n", FLT_MAX);
	printf("float MIN FLT_MIN:~ %f\n", FLT_MIN);
	printf("float precision FLT_DIG:~ %d\n", FLT_DIG);
	printf("10/3 = %.30f\n", g/h);
	puts("");
	printf("Type double :\n\n");
	printf("double MAX DBL_MAX:~ %e\n", DBL_MAX);
	printf("double MIN DBL_MIN:~ %f\n", DBL_MIN);
	printf("double precision DBL_DIG:~ %d\n", DBL_DIG);
	printf("10/3 = %.30f\n", i/j);
	puts("");
	printf("Type long double :\n\n");
	printf("long double MAX LDBL_MAX:~ %Le\n", LDBL_MAX);
	printf("long double MIN LDBL_MIN:~ %Lf\n", LDBL_MIN);
	printf("long double precision LDBL_DIG:~ %d\n", LDBL_DIG);
	printf("10/3 = %.30Le\n", k/l);
	puts("");
	puts("Floats");
	printf("Radix of exponent representation FLT_RADIX:~ %d\n", FLT_RADIX);
	printf("Floating-point founding mode for addition FLT_ROUNDS:~ %d\n", FLT_ROUNDS);
	printf("Decimal digits of precision FLT_DIG:~ %d\n", FLT_DIG);
	printf("Smallest number x such that 1.0 + x != 1.0 FLT_EPSILON:~ %e\n", FLT_EPSILON);
	printf("Number of base FLT_RADIX digits in mantissa FLT_MANT_DIG:~ %d\n", FLT_MANT_DIG);
	printf("Maximum floating-point number FLT_MAX:~ %e\n", FLT_MAX);
	printf("Maximum n such that FLT_RADIX^n-1 is representable FLT_MAX_EXP:~ %d\n", FLT_MAX_EXP);
	printf("Minimum normalised floating-point number FLT_MIN:~ %e\n", FLT_MIN);
	printf("Minimum n such that 10^n is a normalised number FLT_MIN_EXP:~ %d\n", FLT_MIN_EXP);
	puts("");
	puts("Doubles");
	printf("Decimal digits of precision DBL_DIG:~ %d\n", DBL_DIG);
	printf("Smallest number x such that 1.0 + x != 1.0 DBL_EPSILON:~ %e\n", DBL_EPSILON);
	printf("Number of base FLT_RADIX digits in mantissa DBL_MANT_DIG:~ %d\n", DBL_MANT_DIG);
	printf("Maximum double floating-point number DBL_MAX:~ %e\n", DBL_MAX);
	printf("Maximum n such that FLT_RADIX^n-1 is representable DBL_MAX_EXP:~ %d\n", DBL_MAX_EXP);
	printf("Minimum normalised double floating-point number DBL_MIN:~ %e\n", DBL_MIN);
	printf("Minimum n such that 10^n is a normalised number DBL_MIN_EXP:~ %d\n", DBL_MIN_EXP);

	return(0);
}

