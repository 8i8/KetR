/*
 * Exercise 8-1. Rewrite the program cat from Chapter 7 using read, write,
 * open, and close instead of their standard library equivalents. Perform
 * experiments to determine the relative speeds of the two versions.
 * TODO
 */

#include <stdio.h>
#include <stdint.h>

/**
 * cat:	concatinate files version 1.
 */
int main(int argc, char* argv[])
{
	FILE *fp;
	size_t time = SIZE_MAX;

	void filecopy(FILE *, FILE *);

	while(time--)
		if (argc == 1) /* no args copy standard input */
			filecopy(stdin, stdout);
		else
			while (--argc > 0)
				if ((fp = fopen(*++argv, "r")) == NULL) {
					printf("cat: cant't open %s\n", *argv);
					return 1;
				} else {
					filecopy(fp, stdout);
					fclose(fp);
				}
	return 0;
}

void filecopy(FILE *ifp, FILE *ofp)
{
	int c;

	while ((c = getc(ifp)) != EOF)
		putc(c, ofp);
}

