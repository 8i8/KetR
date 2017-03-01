/*
 * 1-9 Write a program that copys its input to its output but replaces any
 * double spaces with a single space.
 */
#include <stdio.h>

int main(int argc, char *argv[])
{
	int i, a, b;

	for (i = 1; i < argc; i++) {

		FILE *file = fopen(argv[i], "r");

		if (file == NULL)
			return 1; //could not open file

		b = 0;

		while ((a = fgetc(file)) != EOF)
		{
			if ((a == ' ') && (b == ' '))
				;
			else 
				printf("%c", a);
			b = a;
		}
		fclose(file);
	}
}
