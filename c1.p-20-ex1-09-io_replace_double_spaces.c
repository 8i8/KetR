/*
 * 1-9 Write a program that copys its input to its output but replaces any
 * double spaces with a single space.
 */
#include <stdio.h>

int main(void)
{
	int a, b;

	b = 0;

	while ((a = getchar()) != EOF)
	{
		// The workings ...
		if ((a == ' ') && (b == ' '))
			;
		else 
			printf("%c", a);
		b = a;
	}

	return 0;
}
