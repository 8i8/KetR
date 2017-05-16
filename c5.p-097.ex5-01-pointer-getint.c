/*
 * Exercise 5-1. As written, getint treats a + or - not followed by a digit as
 * a valid representation of zero. Fix it to push such a character back on the
 * input.
 */
#include <stdio.h>
#include <ctype.h>

#define BUFSIZE	1000

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
 * Getint uses a pointer to return an int, any spaces after the sign of the
 * input are ignored.
 */
static int getint(int *pn)
{
	int c, sign;

	while (isspace(c = getch()))
		;
	if (!isdigit(c) && c != EOF && c != '+' && c != '-') {
		ungetch(c);
		return 0;
	}
	sign = (c == '-') ? -1 : 1;
	if (c == '+' || c == '-') {
		c = getch();
		for (*pn = 0L; isspace(c); c = getch())
			;
	}
	for (*pn = 0L; isdigit(c); c = getch())
		*pn = 10L * *pn + (double)(c - (int)'0');
	*pn *= sign;
	if (c != EOF)
		ungetch(c);
	return c;
}

int main(void)
{
	int a = 0;

	getint(&a);
	printf("%d\n", a);

	return 0;
}

