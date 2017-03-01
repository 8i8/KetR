/*
 * Exercise 1-19. Write a function reverse(s) that reverses the character
 * string s. Use it to write a program that reverses its input a line at a
 * time.
 */
#include <stdio.h>

#define maxlength	1000

void readLineByLine(char line[]);
void reverse(char line[], int length);

int main(void)
{
	char line[maxlength];

	readLineByLine(line);
	return 0;
}

void readLineByLine(char line[])
{
	int c;
	int length = 0;

	while ((c = getchar()) != EOF)
	{
		if (c == '\n') {
			line[length] = '\0';
			reverse(line, length);
			length = 0;
			continue;
		}
		line[length] = c;
		length++;
	}
}

void reverse(char line[], int length)
{
	int i;

	for (i = length; i >= 0; i--)
		printf("%c",line[i]);
	puts("");
}

