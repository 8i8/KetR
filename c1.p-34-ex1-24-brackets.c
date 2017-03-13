/*
 * Exercise 1-24. Write a program to check a C program for rudimentary syntax
 * errors like unmatched parentheses, brackets and braces. Don't forget about
 * quotes, both single and double, escape sequences, and comments. (This
 * program is hard if you do it in full generality.)
 *
 */
#include <stdio.h>

#define MAXLINE		10000

void checkEscapeChar(char input[], int status[], int i);
void checkComments(char input[], int status[], int len, int i);
void checkQuotes(char input[], int status[], int len, int i);
void checkBraces(char input[], int status[], int len, int i);
void checkBrackets(char input[], int status[], int len, int i);
int myGetline(char string[], int lim);

int main(void)
{
	char string[MAXLINE];
	int status[7];
	int len;

	while ((len = myGetline(string, MAXLINE)) > 0)
	{
		int i;
		/*
			status[0] = brackets;
			status[1] = semiColonRequired;
			status[2] = braces;
			status[3] = singleQuotes;
			status[4] = doubleQuotes;
			status[5] = comments;
			status[6] = escaped;
		*/
		for (i = 0; i < len; i++)
		{
			/*
			 * If the escape character is found an we are in
			 * quotes, then skip the next character.
			 */
			checkComments(string, status, len, i);
			if(status[5])
				continue;

			checkEscapeChar(string, status, i);
			if(status[6]) {
				status[6] = 0;
				i++;
				continue;
			}

			checkQuotes(string, status, len, i);
			if (status[3] || status[4])
				continue;

			checkBraces(string, status, len, i);
			checkBrackets(string, status, len, i);
		}
	}
}

void checkEscapeChar(char input[], int status[], int i)
{
	if (input[i] == '\\')
		status[6] = 1;
}

void checkComments(char input[], int status[], int len, int i)
{
	int c;
	int prev;
	static int comments;

	c = input[i];
	if (input[i-1] > 0)
		prev = input[i-1];

	if (c == '*' && prev == '/' && !comments)
		comments = 1;
	else if (c == '/' && prev == '*' && comments)
		comments = 0;

	status[5] = comments;

	if (i == len-2)
	{
		if (comments)
			printf("Your program requires a closing comment marker.\n");
	}
}

void checkQuotes(char input[], int status[], int len, int i)
{
	int c;
	static int singleQuotes;
	static int doubleQuotes;

	c = input[i];

	if (c == 39 && !singleQuotes && !doubleQuotes)
		singleQuotes = 1;
	else if (c == 39 && singleQuotes)
		singleQuotes = 0;

	if (c == '"' && !singleQuotes && !doubleQuotes)
		doubleQuotes = 1;
	else if (c == '"' && doubleQuotes)
		doubleQuotes = 0;

	status[3] = singleQuotes;
	status[4] = doubleQuotes;

	if (i == len-2)
	{
		if (singleQuotes == 1)
		{
			printf("Your program is missing a closing single quote\n");
		}
		else if (doubleQuotes == 1)
		{
			printf("Your program is missing a closing double quote\n");
		}
	}
} 

/*
 * Check program for braces balance, opening and closing.
 */
void checkBraces(char input[], int status[], int len, int i)
{
	int c;
	static int braces = 0;

	c = input[i];

	if (c == '{')
		++braces;
	if (c == '}')
		--braces;

	status[2] = braces;

	if (i == len-2)
	{
		if (braces > 0)
		{
			printf("Your program is missing %d closing braces.\n", braces);
		}
		else if (braces < 0)
		{
			braces = (braces * braces) / braces;
			printf("Your program is missing %d opening braces.\n", braces);
		}
	}
}

/*
 * Check program for bracket balance, opening and closing.
 */
void checkBrackets(char input[], int status[], int len, int i)
{
	int c;
	static int brackets = 0;
	static int semiColonRequired = 0;

	c = input[i];

	if (c == '(')
	{
		++brackets;
		semiColonRequired = 1;
	}
	else if (c == ')')
		--brackets;

	if (c == ';' && semiColonRequired == 1)
		semiColonRequired = 0;

	status[0] = brackets;
	status[1] = semiColonRequired;

	if (i == len-2)
	{
		if (brackets > 0)
			printf("Your program is missing %d closing brackets.\n", brackets);
		else if (brackets < 0)
		{
			brackets = (brackets * brackets) / brackets;
			printf("Your program is missing %d opening brackets.\n", brackets);
		}

		if (semiColonRequired > 0)
			printf("Error, expected ';'.\n");
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

