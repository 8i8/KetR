/*
 * Exercise 1-21. Write a program entab that replaces strings of blanks by the
 * minimum number of tabs and blanks to achieve the same spacing. Use the same
 * tab stops as for detab. When either a tab or a single blank would suffice to
 * reach a tab stop, which should be given preference?
 */
#include <stdio.h>

#define MAXLINE	1000
#define SPACES	8

void addTabs(char str[], char newLine[], int len, int tabSize);
int myGetline(char line[], int lim);

int main(void) {

	int len;
	char line[MAXLINE];
	char newLine[MAXLINE];

	while ((len = myGetline(line, MAXLINE)) > 0)
	{
		addTabs(line, newLine, len, SPACES);
		printf("%s", newLine);
	}

	return 0;
}

/*
 * Replace spaces by the correct combination of both tabs and spaces.
 */
void addTabs(char line[], char newLine[], int len, int tabSize)
{
	int i;
	int j = 0;
	int k;
	int prev = ' ';
	int count = 0;
	int marker;
	int tabs = 0;
	int spaces;

	for (i = 0; i < len; i++)
	{
		/*
		 * Store the previous character, used for the count of
		 * consecutive spaces.
		 */
		//if (i > 0)
		//	prev = line[i-1];

		/*
		 * If the current value and the previous are both spaces, place
		 * a marker on the first of the two and start the count.
		 */
		if ((line[i] == ' ') && (line[i-1] == ' ') && (count == 1))
		{
			marker = i-1;
			count++;
			continue;
		}

		/*
		 * If the current value and the previous are both spaces,
		 * continue the count.
		 */
		else if ((line[i] == ' ') && (prev == ' '))
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
	newLine[j] = '\0';
}

/*
 * Take input line by line.
 */
int myGetline(char str[], int lim)
{
	int i;
	int c;

	for (i = 0; i < lim-1 && (c = getchar()) != 'Q' && c != '\n'; i++)
		str[i] = c;

	if (c == '\n') {
		str[i] = c;
		++i;
	}
	str[i] = '\0';
	return i;
}

