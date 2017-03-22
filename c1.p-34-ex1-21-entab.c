/*
 * Exercise 1-21. Write a program entab that replaces strings of blanks by the
 * minimum number of tabs and blanks to achieve the same spacing. Use the same
 * tab stops as for detab. When either a tab or a single blank would suffice to
 * reach a tab stop, which should be given preference?
 *
 * TODO This code is not functioning yet.
 */

#undef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200112L

#include <stdio.h>

#define MAXLINE	1000
#define TABSIZE	8

static void addTabs(char str[], char newLine[], size_t len, int tabSize);
static size_t getline(char line[], size_t lim);

int main(void) {

	size_t len;
	char line[MAXLINE];
	char newLine[MAXLINE];

	while ((len = getline(line, MAXLINE)) > 0)
	{
		addTabs(line, newLine, len, TABSIZE);
		printf("%s", newLine);
	}

	return 0;
}

/*
 * Replace spaces by the correct combination of both tabs and spaces.
 */
static void addTabs(char line[], char newLine[], size_t len, int tabSize)
{
	size_t i;
	size_t j;
	size_t k;
	size_t count;
	size_t marker;
	size_t tabs;
	size_t spaces;

	j = count = tabs = marker = 0;

	for (i = 0; i < len; i++)
	{
		/*
		 * If the current value and the previous value are both spaces,
		 * and the counting has just started place a marker on the
		 * first of the two array splces and then start to count.
		 */
		if ((line[i] == ' ') && (line[i-1] == ' ') && (count == 1))
		{
			marker = i-1;
			count++;
			continue;
		}

		/*
		 * If the current value and the previous are both spaces,
		 * and the count is under way, continue the count.
		 */
		else if ((line[i] == ' ') && (line[i-1] == ' '))
		{
			count++;
			continue;
		}

		/*
		 * Calculate the quantity of tabs and spaces required to
		 * replace the spaces.
		 */
		else if ((count > 1) && (line[i] != ' '))
		{
			/*
			 * Check distance from the previous tabstop, if greater
			 * than 0, add to the count, this is to account for
			 * that first tab.
			 */
			if(marker % tabSize > 0 || (marker == 0 && i > tabSize))
			{
				tabs++;
				count = count - (tabSize-(marker % tabSize));
			}

			/*
			 * Add to that the number of spaces counted minus the
			 * spaces between the marker and the next tabstop.
			 */
			tabs = tabs + 
				((count - (tabSize-(marker%tabSize))) / tabSize);
			spaces = i % tabSize;

			/* Add the tabs. */
			for (k = 0; k < tabs; k++)
			{
				newLine[j] = '\t';
				j++;
			}

			/* Add the spaces. */
			for (k = 0; k < spaces; k++) {
				newLine[j] = ' ';
				j++;
			}

			/* Reset the values */
			tabs = 0;
			count = 0;

			/* Write the next character */
			newLine[j++] = line[i];
		}
		else
		{
			/* If it is the first space */
			if (line[i] == ' ' && count == 0) {
				count = 1;
			}
			else if (line[i] != ' ' && count == 1) {
				newLine[j++] = ' ';
				newLine[j++] = line[i];
				count = 0;
			}
			/* No spaces, write the character */
			else
				newLine[j++] = line[i];
		}
	}
}

/*
 * Read input line by line.
 */
static size_t getline(char str[], size_t lim)
{
	size_t i;
	int c;

	for (i = 0; i < lim-2 && (c = getchar()) != EOF && c != '\n'; i++)
		str[i] = c;

	if (c == '\n') {
		str[i++] = c;
	}
	str[i] = '\0';
	return i;
}

