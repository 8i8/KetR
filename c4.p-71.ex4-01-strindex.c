/*
 * Exercise 4-1. Write the function strindex(s,t) which returns the position of
 * the rightmost occurrence of t in s, or -1 if there is none.
 */

/* Redefine getline */
#undef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

#define MAXLINE 	1000 /* maximum input line length */

ssize_t strindex(char s[], char t[]);
static int getline(char s[], int lim);

int main(void)
{
	char line[MAXLINE];
	ssize_t found = 0;
	ssize_t pos;

	char pattern[] = "the";

	while(getline(line, MAXLINE) > 0)
	{
		if ((pos = strindex(line, pattern)) >= 0) {
			printf("%02ld:%s", pos, line);
			found++;
		}
	}
	return found;
}

ssize_t strindex(char s[], char t[])
{
	ssize_t i, j, k;
	ssize_t pos;
	pos = -1;

	for (i = 0; s[i] != '\0'; i++) {
		for (j = i, k = 0; t[k] != '\0' && s[j] == t[k]; j++, k++)
			;
		if (k > 0 && t[k] == '\0')
			pos = i;
	}
	return pos;
}

static int getline(char s[], int lim)
{
	size_t i;
	int8_t c;
	i = 0;

	while (--lim > 0 && (c = getchar()) != EOF && c != '\n')
		s[i++] = c;

	if (c == '\n')
		s[i++] = c;

	s[i] = '\0';

	return i;
}

