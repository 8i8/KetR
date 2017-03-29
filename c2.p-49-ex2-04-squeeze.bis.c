/*
 * Exercise 2-4. Write an alternative version of squeeze(s1,s2) that deletes
 * each character in s1 that matches any character in the string s2.
 *
 * Though I have written a solution to this problem, I should look into using
 * this funtion to do the same, it is found in string.h
 *
 * for (i = strchr(s, '#'); i>=0; i--) s[i] = s[i+1];  ???
 *
 * That aside, now redo based upon the squeese function that I apparently
 * missed of first reading.
 */

/* Redefine getline */
#undef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <stdint.h>

static void sortString(char string2[], uint16_t len);
static void squeeze(char string[], char string2[], uint16_t len, uint16_t len2);
static uint16_t getline(char str[], uint16_t lim);

#define MAXLENGTH	65535

int main(void)
{
	uint16_t len;
	uint16_t len2;
	char string[MAXLENGTH];
	char string2[MAXLENGTH];

	printf("Please provide two strings, one after the other ...\n"
			"String 1 :~ ");
	len = getline(string, MAXLENGTH);

	printf("String two, whos characters will be removed from string one.\n");
	len2 = getline(string2, MAXLENGTH);

	sortString(string2, len2);
	squeeze(string, string2, len, len2);

	printf("--> %s\n", string);
}

/*
 * Search for each character in string2 in  string and replace it with '\0',
 * then use a bubble search to send all of the '\0's to the end of the string.
 */
static void squeeze(char string[], char string2[], uint16_t len, uint16_t len2)
{
	size_t i;
	size_t j;

	for (i = 0; i < len2 && string2[i] != '~'; i++)
		for (j = 0; j < len; j++)
			if (string[j] == string2[i])
				string[j] = '\0';

	uint8_t swapped;
	uint8_t temp;

	do {
		swapped = 0;
		for (i = 1; i < len; i++)
		{
			if(string[i-1] < string[i] && string[i-1] == '\0')
			{
				temp = string[i-1];
				string[i-1] = string[i];
				string[i] = temp;
				swapped = 1;
			}
		}
	}
	while (swapped);
}

/*
 * Put the string into alphabetical order and remove any double characters.
 */
static void sortString(char string[], uint16_t len)
{
	size_t i;
	uint8_t swapped;
	uint8_t temp;
	uint8_t count = 0;

	do
	{
		swapped = 0;
		for (i = 1; i < len; i++)
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
static uint16_t getline(char str[], uint16_t lim)
{
	size_t i;
	int c;

	for (i = 0; i < lim-1 && (c = getchar()) != EOF && c != '\n'; i++)
		str[i] = c;

	str[i++] = '\0';
	return i;
}

