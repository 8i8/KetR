/*
 * Ex 1-8 Write a program to count blanks, tab, and newlines.
 */
#include <stdio.h>

int main(void)
{
	int blk;
	int tab;
	int nl;
	int prev;
	int c;

	blk = 0;
	tab = 0;
	nl = 0;

	puts("Please enter your text followed by :q to process."); 

	while ((c = getchar()) != EOF)
	{
		if (c == 'q' && prev == ':')
			break;
		if (c == ' ')
			++blk;
		if (c == '\t')
			++tab;
		if (c == '\n')
			++nl;
		prev = c;
	}

	printf("spaces = %d\n", blk);
	printf("tabs = %d\n", tab);
	printf("newlines = %d\n", nl);

	return 0;
}
