/*
 * Exercise 1-20. Write a program detab that replaces tabs in the input with
 * the proper number of blanks to space to the next tab stop. Assume a fixed
 * set of tab stops, say every n columns. Should n be a variable or a symbolic
 * parameter?
 */

#undef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200112L

#include <stdio.h>

#define MAXLINE	1000
#define TABSIZE	8

static void removeTabs(char str[], char newStr[], size_t len, int spaces);
static size_t getline(char str[], size_t lim);

int main(void) {

	size_t len;
	char line[MAXLINE];
	char newLine[MAXLINE];

	while ((len = getline(line, MAXLINE)) > 0)
	{
		removeTabs(line, newLine, len, TABSIZE);
		printf("%s", newLine);
	}

	return 0;
}

/*
 * Replace tabs with spaces.
 */
static void removeTabs(char line[], char newLine[], size_t len, int spaces)
{
	size_t i;
	int j = 0;
	int k;
	int offset;

	for (i = 0; i < len; i++)
	{
		/*
		 * Advance letter by letter augmenting both i, for the input
		 * array, and j for the output.
		 */
		if(line[i] != '\t') {
			newLine[j] = line[i];
			j++;
		}
		else
		{
			/*
			 * Calculate how many spaces from a tab stop the char
			 * is take that value from the size of a tab so as to
			 * know the distance in spaces untill the next tabstop;
			 * move j forwards writing spaces, until the equivalent
			 * spacing is gained.
			 */
			offset = spaces-(j % spaces);
			for (k = 0; k < offset; k++) {
				newLine[j] = ' ';
				j++;
			}
		}
	}
	newLine[j] = '\0';
}

/*
 * Get input line by line.
 */
static size_t getline(char str[], size_t lim)
{
	size_t i;
	int c;

	for (i = 0; i < lim-1 && (c = getchar()) != EOF && c != '\n'; i++)
		str[i] = c;

	if (c == '\n')
		str[i++] = c;

	str[i] = '\0';
	return i;
}

