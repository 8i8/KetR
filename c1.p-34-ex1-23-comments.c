/*
 * Exercise 1-23. Write a program to remove all comments from a C program.
 * Don't forget to handle quoted strings and character constants properly. C
 * comments don't nest.
 */
#include <stdio.h>

#define MAXSIZE	1000

void stripComments(char input[], char output[], int len);
int myGetline(char string[], int lim);

int main(void)
{
	char input[MAXSIZE];
	char output[MAXSIZE];
	int len;

	while((len = myGetline(input, MAXSIZE)) > 0)
	{
		stripComments(input, output, len);
		printf("%s", output);
	}
}

/*
 * Remove comments from text input.
 */
void stripComments(char input[], char output[], int len)
{
	int i;
	int j = 0;
	int c;
	static int prev = 0;
	static int doubleQuotes = 0;
	static int singleQuotes = 0;
	static int write = 1;

	for(i = 0; i < len; i++)
	{
		/*
		 * Store both the current and the previous values of i to keep
		 * check on the current input status.
		 */
		c = input[i];
		if(i > 0)
			prev = input[i-1];

		/*
		 * Set marker if double quotes are detected.
		 */
		if(c == '"' && !doubleQuotes && !singleQuotes)
			doubleQuotes = 1;
		else if(c == '"' && doubleQuotes)
			doubleQuotes = 0;
		/*
		 * Set marker if single quotes are detected.
		 */
		if(c == 39 && !doubleQuotes && !singleQuotes)
			singleQuotes = 1;
		else if(c == 39 && singleQuotes)
			singleQuotes = 0;

		/*
		 * If not already in either of the above quotes mode, stop
		 * writing to the output array, remove the comment, set the
		 * head 'j' back two spaces to conceal the comment characters.
		 */
		if((prev == '/') && (c == '*') && (!doubleQuotes) && (!singleQuotes) && write)
		{
			write = 0;
			j = j-2;
			continue;
		}

		/*
		 * If not is either quote mode and currently not writing, in
		 * write mode, then start writing.
		 */
		if((prev == '*') && (c == '/') && (!doubleQuotes) && (!singleQuotes) && !write)
		{
			write = 1;
			continue;
		}

		if(write)
		{
			output[j] = input[i];
			j++;
		}
	}
}

int myGetline(char string[], int lim)
{
	int i;
	int c;
	int prev;

	for (i = 0; i < lim-1 && ((c = getchar()) != 'Q'); i++)
	{
		string[i] = c;
		if(i < 0)
			prev = string[i-1];
	}

	if (c == '\n')
		string[i++] = c;

	string[i] = '\0';
	return i;
}

