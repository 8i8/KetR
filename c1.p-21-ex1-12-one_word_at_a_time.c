/*
 * Exercise 1-12. Write a program that prints its input one word per line.
 */
#include <stdio.h>

#define IN	1	/* inside of a word */
#define OUT	0	/* outside of a word */

/* count lines words and characters in input */

int main(void)
{
	int c, prev;

	while ((c = getchar()) != EOF)
	{
		if (c == ' ' && prev == ' ')
			;
		else if (c == ' ')
			printf("\n");
		else 
			printf("%c", c);
		prev = c;
	}

	return 0;
}
