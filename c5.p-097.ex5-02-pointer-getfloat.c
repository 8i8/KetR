/*
 * Exercise 5-2. Write getfloat, the floating-point analog of getint. What type
 * does getfloat return as its function value?
 *
 * getfloat also returns an int, this is used to pass back the EOF character.
 */
#include <stdio.h>
#include <ctype.h>
#include <math.h>

#define BUFSIZE	255

char buf[BUFSIZE];
int buf_p;

/*
 * Get a (possibly pushed back) character.
 */
static int getch(void)
{
	return (buf_p > 0) ? buf[--buf_p] : getchar();
}

/*
 * Push character back on input.
 */
static void ungetch(char c)
{
	if (buf_p >= BUFSIZE)
		printf("ungetch: too many characters\n");
	else
		buf[buf_p++] = c;
}

/*
 * Getfloat uses a pointer to return an int and the return type of the function
 */
static int getfloat(double *pn)
{
	int c, sign;
	double power;
	power = 0;

	while (isspace(c = getch()))
		;
	while (!isdigit(c) && c != EOF && c != '+' && c != '-') {
		ungetch(c);
		return 0;
	}
	sign = (c == '-') ? -1 : 1;
	if (c == '+' || c == '-') {
		c = getch();
		for (*pn = 0; isspace(c); c = getch())
				;
	}

	/* Collect digits before a '.' */
	for (*pn = 0L; isdigit(c); c = getch())
		*pn = 10L * *pn + (double)(c - (int)'0');

	/* Continue to collect digits after a '.' */
	if (c == '.') {
		/* Skip over the character '.' return EOF if required */
		c = getch();
		if (c == EOF)
			return c;
		/* For each extra digit acquired, multiply 'power' by ten */
		for (power = 1L; isdigit(c); c = getch()) {
			*pn = 10L * *pn + (double)(c - (int)'0');
			power *= 10L;
		}
	}

	/*
	 * Set the sign by multiplyinb by sign, and the decimal point by
	 * deviding by power.
	 */
	*pn = sign * *pn / power;

	if (c != EOF)
		ungetch(c);
	return c;
}

int main(void)
{
	double a;

	while (getfloat(&a) != EOF)
		printf("%f\n", a);

	return 0;
}
