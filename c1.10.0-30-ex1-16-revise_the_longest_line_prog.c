/*
 * Exercise 1-16. Revise the main routine of the longest-line program so it
 * will correctly print the length of arbitrary long input lines, and as much
 * as possible of the text.
 */
#include <stdio.h>

int findLongestLine(FILE *file);
int myGetline(char line[], int maxline, FILE *file);
void copy(char to[], char from[]);

/* print the longest input */
int main(int argc, char *argv[])
{
	int i;
	int len;		/* Current line length */
	int max;		/* Maximum length seen so far */
	int length;

	for (i = 1; i < argc; i++)
	{
		FILE *file = fopen(argv[i], "r");

		if (file == NULL)
			return 1; /* Cannot open file */

		length = findLongestLine(file);
		printf("length : %d\n", length - 1);

		char line[length];
		char longest[length];
		rewind(file);

		max = 0;
		while ((len = myGetline(line, length, file)) > 0)
			if (len > max) {
				max = len;
				copy(longest, line);
			}
		if (max > 0)		/* There was a line */
			printf("%s\n", longest);

		fclose(file);
		return 0;
	}
}

int findLongestLine(FILE *file)
{
	int c, len, maxlen;
	len = maxlen = 0;

	while ((c = fgetc(file)) != EOF)
	{
		if (c == '\n') {
			if (len > maxlen)
				maxlen = len;
			len = 0;
		}
		len++;
	}
	return maxlen;
}

/* Getline: Read a line in to s, return its length */
int myGetline(char s[], int lim, FILE *file)
{
	int c, i;

	for (i = 0; i < lim-1 && (c = fgetc(file))!= EOF && c!='\n'; i++)
		s[i] = c;
	if (c == '\n') {
		s[i] = c;
		i++;
	}
	s[i] = '\0';
	return i;
}

/* copy: copr `from` inot `to`; assume `to` is big enough */
void copy(char to[], char from[])
{
	int i;

	i = 0;
	while ((to[i] = from[i]) != '\0')
		i++;
}

