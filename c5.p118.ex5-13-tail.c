/*
 * Exercise 5-13. Write the program tail, which prints the last n lines of its
 * input. By default, n is set to 10, let us say, but it can be changed by an
 * optional argument so that tail -n
 */

#undef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLEN		1000
#define MAXLINES	5000
#define ALLOCSIZE	5000000

static void testwrite(char *string, int start, int len);
static void trimtail(char *lineptr[], int len, int tail);
static int readlines(char *lineptr[], const int maxlines);
static int getline(char *, int);
static char *alloc(const int n);
static void afree(char *p);
static void writelines(char *lineptr[], const int nlines);

static char *lineptr[MAXLINES];			/* Pointer to text lines */
static char allocbuf[ALLOCSIZE];		/* storage for alloc */
static char *allocp = allocbuf;			/* next free position */

/*
 * Sort input lines.
 */
int main(int argc, char *argv[])
{
	int nlines, tail;	/* number of input lines to read */
	tail= 10;

	while (--argc > 1 && (*++argv)[0] == '-')
		if ((*argv)[1] == 'n')
			tail= atof(*++argv);

	if ((nlines = readlines(lineptr, MAXLINES)) >= 0)
	{
		trimtail(lineptr, nlines, tail);
		/* Test that the memory is reset */
		testwrite("Hello World!", ++tail, 11);
		writelines(lineptr, tail);
		return 0;
	} else {
		printf("Error: input to large\n");
		return 1;
	}
}

/*
 * Write test string to char* array.
 */
static void testwrite(char* string, int start, int len)
{
	char *p;

	p = alloc(len);
	lineptr[start] = p;

	strcpy(lineptr[start], string);
}

/*
 * Trim back to required tail length.
 */
static void trimtail(char *lineptr[], int len, int tail)
{
	afree(lineptr[tail]);

	while (tail--)
		lineptr[tail] = lineptr[--len];
}

/*
 * Read input lines, check available space for new line, return line count.
 * Copy the new line into the allocated space and fill lineptr array with
 * pointers to the new lines gathered.
 */
static int readlines(char *lineptr[], const int maxlines)
{
	int len, nlines;
	char *p, line[MAXLEN];

	nlines = 0;

	while ((len = getline(line, MAXLEN)) > 0)
		if (nlines >= maxlines || (p = alloc(len)) == NULL)
			return -1;
		else {
			line[len-1] = '\0'; /* delete newline char*/
			strcpy(p, line);
			lineptr[nlines++] = p;
		}
	return nlines;
}

/*
 * Input from stdin line by line.
 */
static int getline(char *s, int lim)
{
	char *s_in;
	int c;
	s_in = s;

	while (--lim > 0 && (c = getchar()) != EOF && c != '\n')
		*s++ = c;
	if (c == '\n')
		*s++ = c;
	*s = '\0';

	return s - s_in;
}

/*
 * Count memory use for the sort operation.
 */
static char *alloc(const int n)	/* return pointer to  characters */
{
	if (allocbuf + ALLOCSIZE - allocp >= n) { /* if 'n' fits */
		allocp += n;
		return allocp - n;	/* old p */
	} else		/* not enough room */
		return 0;
}

/*
 * Set memory pointer position, free memory.
 */
static void afree(char *p)
{
	if (p >= allocbuf && p < allocbuf + ALLOCSIZE)
		allocp = p;
}

/*
 * Write output lines.
 */
static void writelines(char *lineptr[], const int nlines)
{
	size_t i;

	for (i = 0; i < nlines; i++)
		printf("%s\n", *(lineptr+i));
}

