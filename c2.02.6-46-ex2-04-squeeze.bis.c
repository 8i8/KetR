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
#include <stdio.h>

int myGetline(char string[], int lim);
void sortString(char string2[], int len);
void squeeze(char string[], char string2[], int len, int len2);
int myGetline(char str[], int lim);

#define MAXLENGTH	1000

int main(void)
{
	int len;
	int len2;
	char string[MAXLENGTH];
	char string2[MAXLENGTH];

	printf("Please provide two strings, one after the other ...\n"
			"String 1 :~ ");
	len = myGetline(string, MAXLENGTH);

	printf("String two, whos characters will be removed from string one.\n");
	len2 = myGetline(string2, MAXLENGTH);

	sortString(string2, len2);
	squeeze(string, string2, len, len2);

	printf("--> %s\n", string);
}

/*
 * Search for each character in string2 in  string and replace it with '\0',
 * then use a bubble search to send all of the '\0's to the end of the string.
 */
void squeeze(char string[], char string2[], int len, int len2)
{
	int i;
	int j;

	for (i = 0; i < len2 && string2[i] != '~'; i++)
	{
		for (j = 0; j < len; j++)
		{
			if (string[j] == string2[i])
				string[j] = '\0';
		}
	}

	int swapped;
	int temp;

	do
	{
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
void sortString(char string[], int len)
{
	int i;
	int swapped;
	int temp;

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
			if(string[i-1] == string[i])
				string[i] = '~';
		}
	}
	while (swapped);
}

/*
 * Get input line by line.
 */
int myGetline(char str[], int lim)
{
	int i;
	int c;

	for (i = 0; i < lim-1 && (c = getchar()) != 'Q' && c != '\n'; i++)
		str[i] = c;

	str[i] = '\0';
	return i;
}

