/*
 * Exercise 5-12. Extend entab and detab to accept the shorthand
 *
 *	entab -m +n
 *
 * to mean tab stops every n columns, starting at column m. Choose convenient
 * (for the user) default behavior.
 *
 * ./a.out -m 5 -n 2 < testfile
 *
 * TODO There is an issue in this code, it is not possible to offset the
 * tabstop as requested in the question without writing a shell that can deal
 * with this functioning, as such I have decided to make the margin work as if
 * it were a part of the text file.
 */

#undef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define MAXLINE	255
#define TABSIZE	8
#define MAX_MARGIN 255

static uint8_t countTabs(uint8_t tabsize, uint8_t column);
static uint8_t countSpaces(uint8_t tabsize, uint8_t column);
static void spacesToTabs(char str[], char newLine[], uint8_t len, uint8_t tabsize);
static char* setMargin(char* margin, uint8_t len, uint8_t tabsize);
static uint8_t getline(char line[], uint8_t lim, uint8_t margin);

int main(int argc, char* argv[])
{
	uint8_t len;
	char line[MAXLINE];
	char newLine[MAXLINE];
	char mString[MAX_MARGIN];
	char* m_pt;
	char tabs[8] = { "tabs " };	/* Set system tab size */
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

	if (margin < 0 || tabsize < 0) {
		printf("error: requires positive value\n");
		return 1;
	}

	m_pt = mString;
	m_pt = setMargin(mString, margin, tabsize);
	system(tabs);

	while (!feof(stdin))
	{
		len = getline(line, MAXLINE, margin);
		spacesToTabs(line, newLine, len, tabsize);
		if(len > 1)
			printf("%s", m_pt);
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

	for (i = 0; i <= len; i++)
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
			tabs 	+= countTabs(i-tabs, tabsize);
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
 * Wrinte a string with required tabs and spaces.
 */
static char* setMargin(char* margin, uint8_t len, uint8_t tabsize)
{
	int i = 0;
	uint8_t tabs;
	tabs = len / tabsize;
	len %= tabsize;

	while(tabs--)
		*(margin+i++) = '\t';

	while(len--)
		*(margin+i++) = ' ';

	*(margin+i) = '\0';

	return margin;
}

/*
 * Read input line by line and add a margin if required.
 */
static uint8_t getline(char str[], uint8_t lim, uint8_t margin)
{
	uint8_t i = 0;
	char c;

	for ( ; i < lim-1 && (c = getchar()) != EOF && c != '\n'; i++)
		str[i] = c;

	if (i == margin)
		i -= margin;

	if (c == '\n')
		str[i++] = c;
	str[i] = '\0';
	return i;
}

