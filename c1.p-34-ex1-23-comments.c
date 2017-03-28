/*
 * Exercise 1-23. Write a program to remove all comments from a C program.
 * Don't forget to handle quoted strings and character constants properly. C
 * comments don't nest.
 */

/* redefine getline */
#undef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <stdint.h>

#define MAXSIZE	65535

static void stripComments(char input[], char output[], uint16_t len);
static uint16_t getline(char string[], uint16_t lim);

int main(void)
{
	char input[MAXSIZE];
	char output[MAXSIZE];
	uint16_t len;

	while((len = getline(input, MAXSIZE)) > 0)
	{
		stripComments(input, output, len);
		printf("%s", output);
	}
}

/*
 * Remove comments from text input.
 */
static void stripComments(char input[], char output[], uint16_t len)
{
	uint16_t i;				/* loop for initial array */
	uint16_t j;				/* loop for corrected array */
	uint8_t c;				/* The current char */
	static uint8_t prev = 0;		/* The previous char */
	static uint8_t isDoubleQuotes = 0;
	static uint8_t isSingleQuotes = 0;
	static uint8_t isWrite = 1;

	for(i = j = 0; i < len; i++)
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
		if(c == '"' && !isDoubleQuotes && !isSingleQuotes)
			isDoubleQuotes = 1;
		else if(c == '"' && isDoubleQuotes)
			isDoubleQuotes = 0;
		/*
		 * Set marker if single quotes are detected.
		 */
		if(c == 39 && !isDoubleQuotes && !isSingleQuotes)
			isSingleQuotes = 1;
		else if(c == 39 && isSingleQuotes)
			isSingleQuotes = 0;

		/*
		 * If not already in either of the above quotes mode, stop
		 * writing to the output array, remove the comment, set the
		 * head 'j' back two spaces to conceal the comment characters.
		 */
		if((prev == '/') && (c == '*') && (!isDoubleQuotes) && (!isSingleQuotes) && isWrite)
		{
			isWrite = 0;
			j = j-2;
			continue;
		}

		/*
		 * If not is either quote mode and currently not writing, in
		 * isWrite mode, then start writing.
		 */
		if((prev == '*') && (c == '/') && (!isDoubleQuotes) && (!isSingleQuotes) && !isWrite)
		{
			isWrite = 1;
			continue;
		}

		if(isWrite)
			output[j++] = input[i];
	}
}

/*
 * Input from stdin no more than 65536 bytes.
 */
static uint16_t getline(char string[], uint16_t lim)
{
	uint16_t i; 
	int c;

	for (i = 0; i < lim-1 && ((c = getchar()) != EOF); i++)
		string[i] = c;

	if (c == '\n')
		string[i++] = c;

	string[i] = '\0';
	return i;
}

