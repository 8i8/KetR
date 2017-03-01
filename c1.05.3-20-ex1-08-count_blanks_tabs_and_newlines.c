/*
 * Ex 1-8 Write a program to count blanks, tab, and newlines.
 */
#include <stdio.h>

int main(int argc, char *argv[])
{
	int blk;
	int tab;
	int nl;
	int prev;
	int i, c;

	blk = 0;
	tab = 0;
	nl = 0;

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
}
