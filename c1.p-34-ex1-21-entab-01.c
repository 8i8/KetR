/*
 * Exercise 1-21. Write a program entab that replaces strings of blanks by the
 * minimum number of tabs and blanks to achieve the same spacing. Use the same
 * tab stops as for detab. When either a tab or a single blank would suffice to
 * reach a tab stop, which should be given preference?
 *
 * entab.c
 */

#undef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define MAXLINE	255
#define TABSIZE	8

static uint8_t countTabs(uint8_t tabsize, uint8_t column);
static uint8_t countSpaces(uint8_t tabsize, uint8_t column);
static void spacesToTabs(char str[], char newLine[], uint8_t len, uint8_t tabsize);
static uint8_t getline(char line[], uint8_t lim);

int main(void)
{
	uint8_t len;
	char line[MAXLINE];
	char newLine[MAXLINE];

	while (!feof(stdin))
	{
		len = getline(line, MAXLINE);
		spacesToTabs(line, newLine, len, TABSIZE);
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
	if((line[i-1] == ' ' || line[i-1] == '\t') && (line[i] == ' ' || line[i] == '\t') )
		return i-1;
	return 0;
}

/*
 * Replace spaces by the correct combination of both tabs and spaces.
 */
static void spacesToTabs(char line[], char newLine[], uint8_t len, uint8_t tabsize)
{
	uint8_t i, j, k;
	uint8_t inCount, tabPos, marker, tabs, countStart, remSpaces;
	int8_t spaces;

	i = j = k = inCount = marker = tabs = spaces = countStart = remSpaces = 0;

	for (i = 0; i < len; i++)
	{
		/*
		 * Keep track of the distance from n, the tab column. Will
		 * return 0 when 
		 */
		tabPos = i % tabsize;

		if (!tabPos)
			countStart = spaces = 0; /* reset at the tab stop */

		/*
		 * If both the current and the previous values are spaces,
		 * and the status is true for inCount, place a marker on the
		 * first of the two array spaces and continue.
		 */
		if (inCount == 1 && (line[i] == ' ' || line[i] == '\t'))
		{

			if (line[i-1] == '\t') {
				if (tabPos-1 != 0)
					j--;
				tabs++;
			} else
				spaces++;

			if (line[i] == '\t') {
				if (!tabPos)	/* Not placed on a tabstop */
					j--;
				tabs++;
			} else
				spaces++;

			marker = setMarker(line, i);
			inCount++;
			continue;
		}

		/*
		 * If the current value and the previous are both spaces,
		 * and the marker is already set, continue.
		 */
		else if (inCount && line[i] == ' ')
		{	/* count the spaces between tab stops */
			spaces++;
			continue;
		}

		/*
		 * Deal with tabs following on from several spaces.
		 */
		else if (inCount && line[i] == '\t')
		{
			tabs++;
			/* Remove the spaces pior to tab */
			remSpaces += spaces;
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
			spaces	=  countSpaces(i, tabsize) - tabs - remSpaces;
			tabs 	+= countTabs(i, tabsize);
			tabs 	-= countTabs(marker, tabsize);

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
			if ( (line[i] == ' ' || line[i] == '\t') && !inCount)
				inCount = 1, countStart = tabPos;

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
 * Read input line by line.
 */
static uint8_t getline(char str[], uint8_t lim)
{
	uint8_t i;
	char c;

	for (i = 0; i < lim-1 && (c = getchar()) != EOF && c != '\n'; i++)
		str[i] = c;

	if (c == '\n')
		str[i++] = c;
	str[i++] = '\0';
	return i;
}

