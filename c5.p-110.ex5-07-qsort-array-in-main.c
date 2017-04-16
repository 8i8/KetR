/*
 * Exercise 5-7. Rewrite readlines to store lines in an array supplied by main,
 * rather than calling alloc to maintain storage. How much faster is the
 * program?
 *
 * We se here that the program is no faster when alloc is declaired in main.
 */

/* Redefine getline */
#undef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdint.h>

#define MAXLINES	50000			/* Maxlines to be sorted */
#define MAXLEN		1000			/* max length of any input line */
#define ALLOCSIZE	100000			/* size of available space */
#define BILLION		1000000000L

int readlines(char *lineptr[], int maxlines);
int mainreadlines(char *lineptr[], int maxlines, char alloc[], char *localp);
int getline(char *, int);
char *alloc(int);
char *mainalloc(int n, char alloc[], char *local);
void writelines(char *lineptr[], int nlines);
void qsort(char *lineptr[], int left, int right);

char *lineptr[MAXLINES];			/* Pointer to text lines */
char allocbuf[ALLOCSIZE];			/* storage for alloc */
char *allocp = allocbuf;			/* next free position */

/*
 * Sort input lines.
 */
int main(void)
{
	int nlines;	/* number of input lines to read */

	char mainalloc[ALLOCSIZE];
	char *localp = mainalloc;

	struct timespec start, end;
	size_t i;
	uint64_t time;
	uint64_t time2;
	uint64_t factor;

	factor = 1000000;
	time = 0;

	for (i = 0; i < factor; i++) {

		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);

		if ((nlines = readlines(lineptr, MAXLINES)) >= 0)
		{
			qsort(lineptr, 0, nlines-1);	
			writelines(lineptr, nlines);
		} else {
			printf("Error: input to big to sort\n");
			return 1;
		}

		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
		time += BILLION * (end.tv_sec - start.tv_sec)
					+ end.tv_nsec - start.tv_nsec;

		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);

		if ((nlines = mainreadlines(lineptr, MAXLINES, mainalloc, localp)) >= 0)
		{
			qsort(lineptr, 0, nlines-1);	
			writelines(lineptr, nlines);
		} else {
			printf("Error: input to big to sort\n");
			return 1;
		}

		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
		time2 += BILLION * (end.tv_sec - start.tv_sec)
					+ end.tv_nsec - start.tv_nsec;
	}

	time /= factor;
	time2 /= factor;
	puts("");
	printf("Time 1 -> %lu\n", time);
	printf("Time 2 -> %lu\n", time2);

	return 0;
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
			line[len-1] = '\0'; /* delete newline */
			strcpy(p, line);
			lineptr[nlines++] = p;
		}
	return nlines;
}

/*
 * readlines that does not use alloc.
 */
int mainreadlines(char *lineptr[], int maxlines, char alloc[], char *localp)
{
	int len, nlines;
	char *p, line[MAXLEN];

	nlines = 0;
	while ((len = getline(line, MAXLEN)) > 0)
		if (nlines >= maxlines || (p = mainalloc(len, alloc, localp)) == NULL)
			return -1;
		else {
			line[len-1] = '\0'; /* delete newline */
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
	else if (c == EOF)
		fflush(stdin);
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
 * Count memory use for the sort operation.
 */
char *mainalloc(int n, char alloc[], char *localp)	/* return pointer to  characters */
{
	if (alloc + ALLOCSIZE - localp >= n) { /* if 'n' fits */
		localp += n;
		return localp - n;	/* old p */
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

