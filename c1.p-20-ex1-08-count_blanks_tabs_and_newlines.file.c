/*
 * Ex 1-8 Write a program to count blanks, tab, and newlines.
 */
#include <stdio.h>

int main(int argc, char *argv[])
{
	int blk, tab, nl;
	int i, c;

	blk = 0;
	tab = 0;
	nl  = 0;

	for (i = 1; i < argc; i++) {

		FILE *file = fopen(argv[i],"r");

		if (file == NULL)
			return 1; //could not open file

		while ((c = fgetc(file)) != EOF)
		{
			if (c == ' ')
			{
				++blk;
			}
			if (c == '\t')
			{
				++tab;
			}
			if (c == '\n')
			{
				++nl;
			}
		}

		printf("spaces = %d\n", blk);
		printf("tabs = %d\n", tab);
		printf("newlines = %d\n", nl);
		fclose(file);
	}
}

