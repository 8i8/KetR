/*
 * Exercise 5-13. Write the program tail, which prints the last n lines of its
 * input. By default, n is set to 10, let us say, but it can be changed by an
 * optional argument so that tail -n
 */

#undef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <stdlib.h>

#define MAXLEN	1000

static void getTail(char* s[], int len, int tail);
static int countLines(void);
static char* getline(char* s, int lim);

int main(int argc, char* argv[])
{
	int arSize = 5;
	int input;

	while (--argc > 1 && (*++argv)[0] == '-')
		if ((*argv)[1] == 'n')
			arSize = atof(*++argv);

	char* s[arSize];

	input = countLines();
	getTail(s, input, arSize);


	return 0;
}

static void getTail(char* s[], int len, int tail)
{
	size_t i = 0;
	char* c = NULL;

	while (i++ < len && (c = getline(c, MAXLEN))) 
		if (i >= len - tail)
			**s++ = *c;
}

/*
 * Return the length of the input file.
 */
static int countLines(void)
{
	size_t i;
	char c;

	i = 0;

	while ((c = getchar()) != EOF)
		if (c == '\n')
			i++;
	return i;
}

/*
 * Read stdin, line by line.
 */
static char* getline(char* s, int lim)
{
	char c;

	while(--lim > 0 && (c = getchar()) != EOF && c != '\n')
		*s++ = c;

	if (c == '\n')
		*s++ = '\n';
	*s = '\0';

	return s;
}

/*
 * Count memory use for the sort operation.
 */
static char *alloc(int n)	/* return pointer to  characters */
{
	if (allocbuf + ALLOCSIZE - allocp >= n) { /* if 'n' fits */
		allocp += n;
		return allocp - n;	/* old p */
	} else		/* not enough room */
		return 0;
}

