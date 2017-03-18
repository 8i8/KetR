/*
 * Ex 1-8 Write a program to count blanks, tab, and newlines.
 */
#include <stdio.h>

int main(void)
{
	int blk, tab, nl, c;

	blk = tab = nl = 0;

	while ((c = getchar()) != EOF)
	{
		if (c == ' ')
			++blk;
		if (c == '\t')
			++tab;
		if (c == '\n')
			++nl;
	}

	printf("spaces = %d\n", blk);
	printf("tabs = %d\n", tab);
	printf("newlines = %d\n", nl);

	return 0;
}
