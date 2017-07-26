/*
 * Exercise 7-6. Write a program to compare two files, printing the first line
 * where they differ.
 */
#include <stdio.h>
#include <string.h>

#define MAXLINE	1000

typedef struct FileComp {
	FILE *fp1, *fp2;
	char *file1, *file2;
	char line1[MAXLINE];
	char line2[MAXLINE];
	size_t line_n;
} Files;

/*
 * printErr:	Output, if file open error occurs.
 */
static void printErr(const char *s1, const char *s2)
{
	printf("%s: error, failed to open %s\n", s1, s2);
}

/*
 * printRes:	Output, file comparison results.
 */
static void printRes(const Files comp)
{
	if (comp.line_n == 0)
		printf("Files \"%s\" and \"%s\" are identical.\n",
				comp.file1, comp.file2);
	else {
		printf("Files \"%s\" and \"%s\" differ on line %lu.\n",
				comp.file1, comp.file2, comp.line_n);

		printf("%s\t -> %s\n", comp.file1, comp.line1);
		printf("%s\t -> %s\n", comp.file2, comp.line2);
	}
}

/*
 * scanFiles:	Make file comparison, line by line.
 */
static Files scanFiles(Files comp)
{
	char c, d;
	size_t i, j;
	comp.line_n = 0;

	for (i = 0; c != EOF && d != EOF; i++)
	{
		for (j = 0; (c = getc(comp.fp1)) != EOF && c != '\n'; j++)
			comp.line1[j] = c;
		comp.line1[j] = '\0';

		for (j = 0; (d = getc(comp.fp2)) != EOF && d != '\n'; j++)
			comp.line2[j] = d;
		comp.line2[j] = '\0';

		/*
		 * If lines are not identical store the line number and the
		 * lines and return
		 */
		if (strcmp(comp.line1, comp.line2)) {
			comp.line_n = i+1;
			return comp;
		}
	}
	return comp;
}

/*
 * main:	Program to return the first two dissimilar lines between two
 * 		files.
 */
int main(int argc, char* argv[])
{
	Files comp;
	char *s;

	if (argc > 2)
	{
		/* File pointers retreived from argv[] */
		while (argc-- > 1 && (s = argv[argc]))
			if (argc == 2) {
				if ((comp.fp2 = fopen(s, "r")) == NULL) {
					printErr(argv[0], argv[2]);
					return 1;
				}
			} else if (argc == 1) {
				if ((comp.fp1 = fopen(s, "r")) == NULL) {
					printErr(argv[0], argv[1]);
					return 1;
				}
			}

		/* Store file names */
		comp.file1 = argv[1];
		comp.file2 = argv[2];
		/* Essential program routine starts here */
		comp = scanFiles(comp);
		/* output */
		printRes(comp);

	} else {
		/* Error, wrong input */
		printf("usage:	%s <file1> <file2>\n", *argv);
		return 1;
	}

	return 0;
}

