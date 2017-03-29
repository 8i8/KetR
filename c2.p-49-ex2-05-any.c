/*
 * Exercise 2-5. Write the function any(s1,s2), which returns the first
 * location in a string s1 where any character from the string s2 occurs, or -1
 * if s1 contains no characters from s2. (The standard library function strpbrk
 * does the same job but returns a pointer to the location.)
 */

/* Redefine getline */
#undef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <stdint.h>

#define MAXLINE		255

static uint8_t any(char string[], char string2[]);
static void sortString(char string[], uint8_t len);
static uint8_t getline(char string[], const uint8_t lim);

int main(void)
{
	char string[MAXLINE];
	char string2[MAXLINE];
	uint8_t len;

	printf("Enter string one: ");
	getline(string, MAXLINE);
	printf("%s\n", string);
	printf("Enter string two: ");
	len = getline(string2, MAXLINE);
	printf("%s\n", string2);

	sortString(string2, len);
	any(string, string2);
}

/*
 * Return the first location in string where any character from string2
 * occurs, or -1 if s1 contains no characters from s2.
 */
static uint8_t any(char string[], char string2[])
{
	size_t i;
	size_t j;
	uint8_t isPresent = 0;

	for(i = 0; string2[i] != '\0'; i++)
		for(j = 0; string[j] != '\0'; j++)
			if(string[j] == string2[i]) {
				printf("Character %c as at index %lu of string 1.\n", string2[i], j);
				isPresent = 1;
				break;
			}

	if(isPresent)
		return 0;
	else {
		printf("-1\n");
		return -1;
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
	string[len-count] = '\0';
}

/*
 * Retrieve input from the user.
 */
static uint8_t getline(char string[], const uint8_t lim)
{
	size_t i;
	int8_t c;

	for(i = 0; i < lim-1 && (c = getchar()) != EOF && c != '\n'; i++)
		string[i] = c;

	string[i] = '\0';

	return i;
}

