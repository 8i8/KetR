/*
 * Exercise 1-20. Write a program detab that replaces tabs in the input with
 * the proper number of blanks to space to the next tab stop. Assume a fixed
 * set of tab stops, say every n columns. Should n be a variable or a symbolic
 * parameter?
 */
#include <stdio.h>

#define MAXLINE	1000
#define SPACES	8

void removeTabs(char str[], char newStr[], int len, int spaces);
int myGetline(char str[], int lim);

int main(void) {

	int len;
	char line[MAXLINE];
	char newLine[MAXLINE];

	while ((len = myGetline(line, MAXLINE)) > 0)
	{
		removeTabs(line, newLine, len, SPACES);
		printf("%s", newLine);
	}

	return 0;
}

/*
 * Replace tabs with spaces.
 */
void removeTabs(char str[], char newStr[], int len, int spaces)
{
	int i;
	int j = 0;
	int k;
	int offset;

	for (i = 0; i < len; i++)
	{
/*
 * Advance letter by letter augmenting both i, for the input array, and j for
 * the output.
 */
		if(str[i] != '\t') {
			newStr[j] = str[i];
			j++;
		}
		else
		{
/*
 * Calculate how many spaces from a tab stop the char is take that value from
 * the size of a tab so as to know the distance in spaces untill the next
 * tabstop; move j forwards writing spaces, until the equivalent spacing is
 * gained.
 */
			offset = spaces-(j % spaces);
			for (k = 0; k < offset; k++) {
				newStr[j] = ' ';
				j++;
			}
		}
	}
}

/*
 * Get input line by line.
 */
int myGetline(char str[], int lim)
{
	int i;
	int c;

	for (i = 0; i < lim-2 && (c = getchar()) != 'Q' && c != '\n'; i++)
		str[i] = c;

	if (c == '\n') {
		str[i] = c;
		++i;
	}
	str[i] = '\0';
	return i;
}

