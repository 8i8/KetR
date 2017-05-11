/*
 * Exercise 5-12. Extend entab and detab to accept the shorthand
 *
 *	entab -m +n
 *
 * to mean tab stops every n columns, starting at column m. Choose convenient
 * (for the user) default behavior.
 *
 * TODO There is a bug in this code, when the -m margin is set to one more than
 * a number that is modulo the tab size, if there are already spaces on the
 * line the first tab after the margin includes an unrequired space.
 */

#undef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define MAXLINE	255
#define TABSIZE	8

static uint8_t countTabs(uint8_t tabsize, uint8_t column);
static uint8_t countSpaces(uint8_t tabsize, uint8_t column);
static void spacesToTabs(char str[], char newLine[], uint8_t len, uint8_t tabsize);
static uint8_t getline(char line[], uint8_t lim, uint8_t margin);

int main(int argc, char* argv[])
{
	uint8_t len;
	char line[MAXLINE];
	char newLine[MAXLINE];
	char tabs[8] = { "tabs " };
	int tabsize = TABSIZE;
	int margin = 0;

	while (--argc > 1 && (*++argv)[0] == '-')
	{
		if ((*argv)[1] == 'm')
			margin = atof(*++argv), --argc;
		else if ((*argv)[1] == 'n') {
			tabsize = atof(*++argv), --argc;
			strcat(tabs, *argv);
		}
	}

	system(tabs);

	while (!feof(stdin))
	{
		len = getline(line, MAXLINE, margin);
		spacesToTabs(line, newLine, len, tabsize);
		printf("%s", newLine);
	}

	return 0;
}

/*
 * Return the previous tabstop.
 */
static uint8_t countTabs(uint8_t column, uint8_t tabsize)
{
	return column / tabsize;
}

/*
 * Return the spaces used since the previous tabstop.
 */
static uint8_t countSpaces(uint8_t column, uint8_t tabsize)
{
	return column % tabsize;
}

/*
 * If there are more than one spaces, return the column place of the first.
 */
static uint8_t setMarker(char line[], uint8_t i)
{
	if(line[i-1] == ' ' && line[i] == ' ')
		return i-1;
	return 0;
}

/*
 * Replace spaces by the correct combination of both tabs and spaces.
 */
static void spacesToTabs(char line[], char newLine[], uint8_t len, uint8_t tabsize)
{
	uint8_t i, j, k;
	uint8_t inCount, marker, tabs, spaces;

	i = j = k = inCount = marker = tabs = spaces = 0;

	for (i = 0; i < len; i++)
	{
		/*
		 * If both the current and the previous values are spaces,
		 * and the status is true for inCount, place a marker on the
		 * first of the two array spaces and continue, inCount is
		 * augmented to 2 and as such will not be effected in the next
		 * itteration.
		 */
		if (inCount == 1 && line[i] == ' ')
		{
			marker = setMarker(line, i);
			inCount++;
			continue;
		}

		/*
		 * If the current value and the previous are both spaces,
		 * and the marker is already set, continue.
		 */
		else if (inCount && line[i] == ' ')
			continue;

		/*
		 * Deal with tabs following on from several spaces.
		 */
		else if (inCount && line[i] == '\t') {
			tabs++;
			line[i] = ' ';
			continue;
		}

		/*
		 * Calculate the quantity of tabs and spaces required.
		 */
		else if (inCount > 1)
		{
			/*
			 * Get the figures. To achieve this, get tabstop count
			 * upto the present position, subtract from that the
			 * tab count upto the marker and then add the remaining
			 * spaces.
			 */
			tabs 	+=  countTabs(i, tabsize);
			tabs 	-= countTabs(marker, tabsize);
			spaces	=  countSpaces(i, tabsize);

			/* Add the tabs. */
			while (tabs > 0)
				newLine[j++] = '\t', tabs--;

			/* Add the spaces. */
			while (spaces > 0)
				newLine[j++] = ' ', spaces--;

			/* Reset status */
			inCount = 0;

			/* Write the next character */
			newLine[j++] = line[i];
		}
		else
		{
			/*
			 * If it is the first space, set the status and
			 * continue with out writing the space character.
			 */
			if (line[i] == ' ' && !inCount)
				inCount = 1;

			/*
			 * If there was only one space, catch up by writing
			 * that space and also the character that follows it.
			 */
			else if (line[i] != ' ' && inCount == 1) {
				newLine[j++] = ' ';
				newLine[j++] = line[i];
				inCount = 0;
			}
			/* No spaces, write the character */
			else
				newLine[j++] = line[i];
		}
	}
}

/*
 * Read input line by line and add a margin if required.
 */
static uint8_t getline(char str[], uint8_t lim, uint8_t margin)
{
	uint8_t i = 0;
	char c;

	for ( ; i < margin && i < lim-1; i++)
		str[i] = ' ';

	for ( ; i < lim-1 && (c = getchar()) != EOF && c != '\n'; i++)
		str[i] = c;

	if (i == margin)
		i -= margin;

	if (c == '\n')
		str[i++] = c;
	str[i++] = '\0';
	return i;
}

