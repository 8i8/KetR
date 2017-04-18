/*
 * Exercise 5-7. Rewrite readlines to store lines in an array supplied by main,
 * rather than calling alloc to maintain storage. How much faster is the
 * program?
 */

/* Redefine getline */
#undef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <string.h>

#define MAXLINES	500			/* Maxlines to be sorted */
#define MAXLEN		1000			/* max length of any input line */
#define ALLOCSIZE	1000			/* size of available space */

int readlines(char *lineptr[], char *allocbuf, int nlines, int lim);
int getline(char *s, int nlines, int lim);
char *alloc(int);
void writelines(char *lineptr[], int nlines);
void qsort(char *lineptr[], int left, int right);

char *lineptr[MAXLINES];			/* Pointer to text lines */

/*
 * Sort input lines.
 */
int main(void)
{
	char allocbuf[ALLOCSIZE][MAXLEN];
	char* allocp;
	int nlines;

	allocp = (char*)allocbuf;

	nlines = 0;

	while (nlines < MAXLINES && !feof(stdin))
		getline(allocp, nlines++, MAXLEN);

	if ((nlines = readlines(lineptr, allocp, nlines, MAXLEN)) >= 0) {
		qsort(lineptr, 0, nlines-1);
		writelines(lineptr, nlines);
		return 0;
	} else {
		printf("Error: input to big to sort\n");
		return 1;
	}
}

/*
 * Assigns the address of each line of text in allocbuf[] to a pointer in the
 * array of pointers *lineptr[].
 */
int readlines(char *lineptr[], char *allocp, int nlines, int lim)
{
	char *p;
	int i = 0;

	while (i < nlines)
		if (nlines >= MAXLINES)
			return -1;
		else {
			p = allocp+nlines*lim;
			lineptr[i++] = p;
			printf("lineptr -> %s\n", lineptr[i-1]);
		}

	return nlines;
}

/*
 * Input from stdin line by line.
 */
int getline(char *s, int nlines, int lim)
{
	char c;
	char* s_in;
	s_in = s;

	s += (nlines*lim);

	while (--lim > 0 && (c = getchar()) != EOF && c != '\n')
		*s++ = c;
	if (c == '\n')
		*s++ = c;
	*s = '\0';

	return s - s_in;
}

/*
 * Write output lines.
 */
void writelines(char *lineptr[], int nlines)
{
	while (nlines-- > 0)
		printf("%d -> %s\n", nlines, *lineptr++);
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

