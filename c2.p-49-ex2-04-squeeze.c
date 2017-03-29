/*
 * Exercise 2-4. Write an alternative version of squeeze(s1,s2) that deletes
 * each character in s1 that matches any character in the string s2.
 *
 * Two strings can be sent into the program using stdin from the command line
 * by running printf into | pipe, such as:
 *
 * printf "Line one \n line two" | ./a.out
 */

/* Redefine getline */
#undef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <stdint.h>

static void sortString(char string2[], uint8_t len);
static void squeeze(char string[], char string2[]);
static uint8_t getline(char str[], const uint8_t lim);

#define MAXLENGTH	255

int main(void)
{
	char string[MAXLENGTH];
	char string2[MAXLENGTH];
	uint8_t len;

	printf("Please provide two strings, one after the other ...\n"
			"String 1 :~ \n");
	getline(string, MAXLENGTH);

	printf("String two, whos characters will be removed from string one.\n");
	len = getline(string2, MAXLENGTH);

	sortString(string2, len);
	squeeze(string, string2);

	printf("--> %s\n", string);
}

/*
 * Remove all of the characters found in string two, from string one.
 */
static void squeeze(char string[], char string2[])
{
	size_t i, j, k;

	for (i = 0; string2[i] != '\0'; i++)
	{
		for (j = k = 0; string[j] != '\0'; j++)
			if (string[j] != string2[i])
				string[k++] = string[j];
		string[k] = '\0';
	}
}

/*
 * Put the string into alphabetical order and remove any double characters.
 */
static void sortString(char string[], uint8_t len)
{
	size_t i;
	uint8_t swapped;
	uint8_t temp;
	uint8_t count = 0;

	do {
		swapped = 0;
		for (i = 1; string[i] != '\0'; i++)
		{
			if(string[i-1] > string[i])
			{
				temp = string[i-1];
				string[i-1] = string[i];
				string[i] = temp;
				swapped = 1;
			}
			/*
			 * Convert any duplicates to ~ so that they are moved
			 * to the end of the array
			 */
			if(string[i-1] == string[i] && string[i] != '~') {
				string[i] = '~';
				count++;
			}
		}
	}
	while (swapped);
	/* Remove the doubles that were converted to ~ */
	string[len-1-count] = '\0';
}

/*
 * Get input line by line.
 */
static uint8_t getline(char str[], const uint8_t lim)
{
	size_t i;
	int8_t c;

	for (i = 0; i < lim-1 && (c = getchar()) != EOF && c != '\n'; i++)
		str[i] = c;

	str[i++] = '\0';

	fflush(stdin);

	return i;
}

