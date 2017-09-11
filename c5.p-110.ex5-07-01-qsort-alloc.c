/*
 * Exercise 5-7. Rewrite readlines to store lines in an array supplied by main,
 * rather than calling alloc to maintain storage. How much faster is the
 * program?
 *
 * Code prior to testing the speed.
 */

/* Redefine getline */
#undef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <string.h>

#define MAXLINES	5000			/* Maxlines to be sorted */
#define MAXLEN		1000			/* max length of any input line */
#define ALLOCSIZE	5000			/* size of available space */

int readlines(char *lineptr[], int maxlines);
int getline(char *, int);
char *alloc(int);
void writelines(char *lineptr[], int nlines);
void qsort(char *lineptr[], int left, int right);

char allocbuf[ALLOCSIZE];			/* storage for alloc */
char *allocp = allocbuf;			/* next free position */

/*
 * Sort input lines.
 */
int main(void)
{
	char *lineptr[MAXLINES];		/* Pointer to text lines */
	int nlines;				/* number of input lines to read */

	if ((nlines = readlines(lineptr, MAXLINES)) >= 0) {
		qsort(lineptr, 0, nlines-1);
		writelines(lineptr, nlines);
		return 0;
	} else {
		printf("Error: input to big to sort\n");
		return 1;
	}
}

/*
 * Read input lines, check available space for new line, return line count.
 * Copy the new line into the allocated space and fill lineptr array with
 * pointers to the new lines gathered.
 */
int readlines(char *lineptr[], int maxlines)
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
int getline(char *s, int lim)
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
char *alloc(int n)	/* return pointer to  characters */
{
	if (allocbuf + ALLOCSIZE - allocp >= n) { /* if 'n' fits */
		allocp += n;
		return allocp - n;	/* old p */
	} else		/* not enough room */
		return 0;
}

/*
 * Write output lines.
 */
void writelines(char *lineptr[], int nlines)
{
	while (nlines-- > 0)
		printf("%s\n", *lineptr++);
}

/*
 * Sort v[left]...v[right] into increasing order.
 */
void qsort(char *v[], int left, int right)
{
	int i, last;
	void swap(char *v[], int i, int j);

	if (left >= right)	/* do nothing if array cantains */
		return;		/* fewer than two eliments */
	swap(v, left, (left + right)/2);
	last = left;
	for (i = left+1; i <= right; i++)
	        if (strcmp(v[i], v[left]) < 0)
	 	       swap(v, ++last, i);
	swap(v, left, last);
	qsort(v, left, last-1);
	qsort(v, last+1, right);
}

/*
 * Interchange v[i] and v[j]
 */
void swap(char *v[], int i, int j)
{
	char *temp;

	temp = v[i];
	v[i] = v[j];
	v[j] = temp;
}

