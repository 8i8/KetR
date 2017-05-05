/*
 * Exercise 4-7. Write a routine ungets(s) that will push back an entire string
 * onto the input. Should ungets know about buf and bufp, or
 * should it just use ungetch?
 */
#include <stdio.h>
#include <string.h>

static int getch(void);
static void ungetch(int);
static void ungets(char s[]);

int main(void)
{
	char c;
	char *s = "This is a test string";

	ungets(s);

	while ((c = getch()) != EOF)
		putchar(c);

	return 0;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Buffer.
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
#define BUFSIZE	100

static char buf[BUFSIZE];	/* Buffer for next ungetch */
static int bufp = 0;		/* next free position in buf */

static int getch(void)		/* get a (possibly pushed back) character */
{
	return (bufp > 0) ? buf[--bufp] : getchar();
}

static void ungetch(int c)	/* push character back on input */
{
	if (bufp >= BUFSIZE)
		printf("ungetch: too many characters\n");
	else
		buf[bufp++] = c;
}

static void ungets(char s[])	/* Push string back into the input buffer */ 
{
	size_t i = strlen(s);

	while (i > 0)
		ungetch(s[--i]);
}

