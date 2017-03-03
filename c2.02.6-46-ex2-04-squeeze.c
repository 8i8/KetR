/*
 * Exercise 2-4. Write an alternative version of squeeze(s1,s2) that deletes
 * each character in s1 that matches any character in the string s2.
 *
 * for (i = strchr(s, '#'); i>=0; i--) s[i] = s[i+1];  ???
 *
 */
#include <stdio.h>

int myGetline(char string[], int lim);
void sortString(char string2[], int len);
void squeeze(char string[], char string2[], int len);

#define MAXLENGTH	1000

int main(void)
{
	int len;
	int len2;
	char string[MAXLENGTH];
	char string2[MAXLENGTH];

	printf("Please provide two strings, one after the other ...\n"
			"String 1 :~ ");

	while((len = myGetline(string, MAXLENGTH)) > 0)
		sortString(string, len);

	printf("String two, whos characters will be removed from string one.\n");
	while((len2 = myGetline(string2, MAXLENGTH)) > 0)
		sortString(string2, len2);
}

void squeeze(char string[], char string2[], int len)
{
}

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
	}while (swapped);
}

int myGetline(char string[], int lim)
{
	int i;
	int c;

	for (i = 0; i < lim-1 && (c = getchar()) != 'Q' && c != '\n'; i++)
		string[i] = c;

	if (c == '\n')
		string[i++] = '\n';

	string[i] = '\0';

	return i;
}

