/*
 * 1-9 Write a program that copys its input to its output but replaces any
 * double spaces with a single space.
 */
#include <stdio.h>

int main(int argc, char *argv[])
{
	int i, a, b;

	b = 0;

	while ((a = getchar()) != 'q')
	{
		// End program with :q
		if ( c == 'q') && (b == ':')
			break;

		// The workings ...
		if ((a == ' ') && (b == ' '))
			;
		else 
			printf("%c", a);
		b = a;
	}
}
