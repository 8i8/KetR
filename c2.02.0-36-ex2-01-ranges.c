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
	printf("For char max:~ %d\n", UCHAR_MAX);
	puts("");
	printf("For char max:~ %d\n", SCHAR_MAX);
	printf("For char min:~ %d\n", SCHAR_MIN);
	puts("");
	printf("For char max:~ %d\n", CHAR_MAX);
	printf("For char min:~ %d\n", CHAR_MIN);
	puts("");
	printf("Type short int :\n\n");
	printf("For unsigned short:~ %hu\n", USHRT_MAX);
	printf("%hd + 1 = %hd\n", a, a+1);
	puts("");
	printf("For short:~ %hd\n", SHRT_MAX);
	printf("For short:~ %hd\n", SHRT_MIN);
	printf("%hd + 1 = %hd\n", b, b+1);
	puts("");
	printf("Type int :\n\n");
	printf("For unsigned int:~ %u\n", UINT_MAX);
	printf("%u + 1 = %u\n", c, c+1);
	puts("");
	printf("For int:~ %d\n", INT_MAX);
	printf("For int:~ %d\n", INT_MIN);
	printf("%d + 1 = %d\n", d, d+1);
	puts("");
	printf("Type long int :\n\n");
	printf("For unsigned long:~ %lu\n", ULONG_MAX);
	printf("%lu + 1 = %lu\n", e, e+1);
	puts("");
	printf("For int:~ %ld\n", LONG_MAX);
	printf("For int:~ %ld\n", LONG_MIN);
	printf("%ld + 1 = %ld\n", f, f+1);
	puts("");
	printf("Type floats :\n\n");
	printf("float MAX:~ %f\n", FLT_MAX);
	printf("float MIN:~ %f\n", FLT_MIN);
	printf("float precision:~ %d\n", FLT_DIG);
	printf("10/3 = %.30f\n", g/h);
	puts("");
	printf("Type double :\n\n");
	printf("double MAX:~ %f\n", DBL_MAX);
	printf("double MIN:~ %f\n", DBL_MIN);
	printf("double precision:~ %d\n", DBL_DIG);
	printf("10/3 = %.30f\n", i/j);
	puts("");
	printf("Type long double :\n\n");
	printf("long double MAX:~ %Le\n", LDBL_MAX);
	printf("long double MIN:~ %Lf\n", LDBL_MIN);
	printf("long double precision:~ %d\n", LDBL_DIG);
	printf("10/3 = %.30Le\n", k/l);

	return(0);
}

