/*
 * Exercise 2-5. Write the function any(s1,s2), which returns the first
 * location in a string s1 where any character from the string s2 occurs, or -1
 * if s1 contains no characters from s2. (The standard library function strpbrk
 * does the same job but returns a pointer to the location.)
 */
#include <stdio.h>

#define MAXLINE		1000

int any(char string[], char string2[], int len, int len2);
int getLine(char string[], int lim);

int main(void)
{
	char string[MAXLINE];
	char string2[MAXLINE];
	int len;
	int len2;

	puts("Enter string one: ");
	len = getLine(string, MAXLINE);
	puts("Enter string two: ");
	len2 = getLine(string2, MAXLINE);

	any(string, string2, len, len2);
}

/*
 * Return the first location in string string where any character from string2
 * occurs, or -1 if s1 contains no characters from s2.
 */
int any(char string[], char string2[], int len, int len2)
{
	int i;
	int j;
	int isPresent = 0;

	for(i = 0; i < len; i++)
	{
		for(j = 0; j < len2; j++)
		{
			if(string[i] == string2[j])
			{
				printf("Character %c -- index --> %d in string 1.\n", string[i], i);
				isPresent = 1;
			}
		}
	}

	if(!isPresent)
		return -1;
	else
		return 0;
}

/*
 * Retrieve input from the user.
 */
int getLine(char string[], int lim)
{
	int i;
	int c;

	for(i = 0; i < lim-1 && (c = getchar()) != '\n'; i++)
		string[i] = c;

	string[i] = '\0';

	return i;
}

