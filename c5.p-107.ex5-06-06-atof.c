/*
 * Exercise 5-6. Rewrite appropriate programs from earlier chapters and
 * exercises with pointers instead of array indexing. Good possibilities
 * include getline (Chapters 1 and 4), atoi, itoa, and their variants (Chapters
 * 2, 3, and 4), reverse (Chapter 3), and strindex and getop (Chapter 4).
 *
 * atof
 */
#include <stdio.h>
#include <ctype.h>

/*
 * Convers string s into double.
 */
static double atof(char *s)
{
	double val, power;
	int sign;

	val = 0L;
	power = 1.0L;

	while (isspace(*s))
		*s++;

	sign = (*s == '-') ? -1 : 1;
	if (*s == '+' || *s == '-')
		*s++;
	while (isdigit(*s))
		val = 10.0L * val + (double)(*s++ - '0');
	if (*s == '.')
		*s++;
	while (isdigit(*s)) {
		val = 10.0L * val + (double)(*s++ - '0');
		power *= 10.0L;
	}

	return sign * val / power;
}

int main(void)
{
	char s[] = "123.45";
	double result;

	result = atof(s);

	printf("result -> %f", result);

	return 0;
}

