/*
 * Exercise 1-12. Write a program that prints its input one word per line.
 */
#include <stdio.h>

#define IN	1	/* inside of a word */
#define OUT	0	/* outside of a word */

/* count lines words and characters in input */

int main(int argc, char *argv[])
{
	int i, c, s;

	for (i = 1; i < argc; i++) {

		FILE *file = fopen(argv[i], "r");

		if (file == NULL)
			return 1; //Could not open file

		while ((c = fgetc(file)) != EOF)
		{
			if (c == ' ' && s == ' ')
				;
			else if (c == ' ')
				printf("\n");
			else 
				printf("%c",c);
			s = c;
		}
		fclose(file);
	}
}
