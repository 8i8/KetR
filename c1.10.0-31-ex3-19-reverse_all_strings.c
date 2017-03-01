/*
 * Exercise 1-19. Write a function reverse(s) that reverses the character
 * string s. Use it to write a program that reverses its input a line at a
 * time.
 */
#include <stdio.h>

#define maxlength	1000

void readInput(void);
int readLineByLine(int line[]);
void reverse(int line[], int length);

int main(int argc, char *argv[])
{
	readInput();
	return 0;
}

void readInput()
{
	int line[maxlength];
	int length;

	length = readLineByLine(line);
}

int readLineByLine(int line[])
{
	int c;
	int length = 0;

	while ((c = getchar()) != EOF)
	{
		if (c == '\n') {
			reverse(line, length);
			length = 0;
		}
		line[length] = c;
		length++;
	}
}

void reverse(int line[], int length)
{
	int i;

	for (i = length; i >= 0; i--)
		printf("%c",line[i]);

	printf("\n");
}

