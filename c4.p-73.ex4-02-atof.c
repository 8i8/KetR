/*
 * Exercise 4-2. Extend atof to handle scientific notation of the form
 * 123.45e-6 where a floating-point number may be followed by e or E and an
 * optionally signed exponent.
 */

/* Redefine getline */
#undef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <ctype.h>

#define MAXLINE	1000

/*
 * Convert from char to float, read decimal points, the + - operators and
 * scientific notation.
 */
double atof(char s[])
{
	double val, power;
	int i, sign, sci;

	/* If the first char are whitespace, skip them. */
	for (i = 0; isspace(s[i]); i++)
		;

	/* If the number is preceded by the minus operator, record this. */
	sign = (s[i] == '-') ? -1 : 1;

	/* If the next character is an operator, pass over it. */
	if (s[i] == '+' || s[i] == '-')
		i++;

	/*
	 * If the char is a digit, change it to its integer equivalent, add it
	 * to val and multiply the result by ten.
	 */
	for (val = 0L; isdigit(s[i]); i++)
		val = 10L * val + (double)(s[i] - '0');

	/* If the next value is a decimal point, skip over it. */
	if (s[i] == '.')
		i++;

	/*
	 * For the remaining char, change them into integers, add them to val,
	 * and multiply by ten. Divide val by 10 and multiply by the sign
	 * stored in sign.
	 */
	for (power = 1L; isdigit(s[i]); i++) {
		val = 10L * val + (s[i] - '0');
		power *= 10L;
	}
	val = sign * val / power;

	/*
	 * If there is a scientific exponant, read this.
	 */
	if (tolower(s[i]) == 'e') {
		i++;
		sign = (s[i] == '-') ? -1 : 1;
		if (s[i] == '+' || s[i] == '-')
			i++;
		/*
		 * For each digit after the e scientific exponent operator,
		 * change the char to its corresponding integer and multiply by
		 * ten; Either multiply or devide 'val' by the result,
		 * dependant upon the sign.
		 */
		for (sci = 0; isdigit(s[i]); i++)
			sci = 10 * sci + (int)(s[i] - '0');
		if (sign == 1)
			while (sci-- > 0)
				val *= 10;
		else
			while (sci-- > 0)
				val /=10;
	}
	return val;
}

int main(void)
{
	double sum, atof(char []);
	char line[MAXLINE];
	int getline(char line[], int max);

	sum = 0;
	while (getline(line, MAXLINE) > 0)
		printf("\t%g\n", sum += atof(line));

	return 0;
}

