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

static int getline(char* s, int lim);

int main(int argc, char* argv[])
{
	int arSize;
	char s[MAXLEN];

	while (--argc > 1 && (*++argv)[0] == '-')
		if ((*argv)[1] == 'n')
			arSize = atof(*++argv);

	char* tail[arSize];

	getline(s, MAXLEN);
	printf("%s", s);

	return 0;
}

/*
 * Read stdin, line by line.
 */
static int getline(char* s, int lim)
{
	char c;
	char* s_pt = s;

	while(--lim > 0 && (c = getchar()) != EOF && c != '\n')
		*s++ = c;
	if (c == '\n')
		*s++ = '\n';
	*s = '\0';

	return s - s_pt;
}

