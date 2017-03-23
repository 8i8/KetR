/*
 * Exercise 1-20. Write a program detab that replaces tabs in the input with
 * the proper number of blanks to space to the next tab stop. Assume a fixed
 * set of tab stops, say every n columns. Should n be a variable or a symbolic
 * parameter?
 */

#undef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <stdint.h>

#define MAXLINE	255
#define TABSIZE	8

static uint8_t countSpaces(uint8_t tabsize, uint8_t column);
static void tabsToSpaces(char str[], char newStr[], uint8_t len, uint8_t spaces);
static uint8_t getline(char str[], uint8_t lim);

int main(void) {

	uint8_t len;
	char line[MAXLINE];
	char newLine[MAXLINE];

	while (!feof(stdin))
	{
		len = getline(line, MAXLINE);
		tabsToSpaces(line, newLine, len, TABSIZE);
		printf("%s", newLine);
	}

	return 0;
}

/*
 * Return the spaces passed since the previous tabstop.
 */
static uint8_t countSpaces(uint8_t column, uint8_t tabsize)
{
	return column % tabsize;
}

/*
 * Replace tabs with spaces.
 */
static void tabsToSpaces(char line[], char newLine[], uint8_t len, uint8_t tabsize)
{
	uint8_t i;
	uint8_t j;
	uint8_t k;
	uint8_t offset;

	i = j = k = 0;

	for (i = 0; i < len; i++)
	{
		/*
		 * Advance letter by letter augmenting both i, for the input
		 * array, and j for the output.
		 */
		if(line[i] != '\t')
			newLine[j++] = line[i];

		else
		{
			/*
			 * Calculate how many spaces from the previous tabstop
			 * the current char is; Subtract that value from the
			 * size of a tab; This gives the distance in spaces to
			 * the next tabstop. Move j forwards by that many
			 * places, each time writing a char space, until the
			 * equivalent space that was previously occupied by the
			 * tab, is filled with spaces.
			 */
			offset = tabsize - countSpaces(j, tabsize);
			for (k = 0; k < offset; k++)
				newLine[j++] = ' ';
		}
	}
}

/*
 * Get input line by line.
 */
static uint8_t getline(char str[], uint8_t lim)
{
	uint8_t i;
	int c;

	for (i = 0; i < lim-1 && (c = getchar()) != EOF && c != '\n'; i++)
		str[i] = c;
	if (c == '\n')
		str[i++] = c;
	str[i++] = '\0';

	return i;
}

