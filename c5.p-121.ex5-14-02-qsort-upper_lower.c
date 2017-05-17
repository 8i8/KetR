/*
 * Exercise 5-14. Modify the sort program to handle a -r flag, which indicates
 * sorting in reverse (decreasing) order. Be sure that -r works with -n.
 */

/* Redefine getline */
#undef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define MAXLEN		1000			/* max length of any input line */
#define MAXLINES	5000			/* Maxlines to be sorted */
#define ALLOCSIZE	5000000			/* size of available space */

static int readlines(char *lineptr[], size_t maxlines);
static int getline(char *, size_t);
static char *alloc(size_t);
static void writelines(char *lineptr[], size_t nlines);
static void _qsort(void *lineptr[], int left, int right,
		int (*comp)(void *, void *), int direction);
static void swap(void *v[], size_t i, size_t j);
static int fldcse(char *s1, char *s2);
static int numcmp(char *s1, char *s2);

static char *lineptr[MAXLINES];			/* Pointer to text lines */
static char allocbuf[ALLOCSIZE];		/* storage for alloc */
static char *allocp = allocbuf;			/* next free position */

enum function { alpha, numeric, fold };

/*
 * Sort input lines.
 */
int main(int argc, char *argv[])
{
	int nlines;		/* number of input lines to read */
	int reverse = 0;	/* reverse the order of the search */
	int func = alpha;
	int c;

	/*
	 * Get setting for search criteria.
	 */
	while (--argc > 0 && (*++argv)[0] == '-')
		while ((c = *++argv[0]))
			switch (c) {
				case 'n':
					func = numeric;
					break;
				case 'r':
					reverse = 1;
					break;
				case 'f':
					func = fold;
					break;
				default:
					break;
			}
			
	/*
	 * Run quicksort with appropriate function and settings.
	 */
	if ((nlines = readlines(lineptr, MAXLINES)) >= 0) {
		switch (func) {
			case alpha:
				_qsort((void **) lineptr, 0, nlines-1,
						(int (*)(void*, void*))(strcmp),
						reverse);
				break;
			case numeric:
				_qsort((void **) lineptr, 0, nlines-1,
						(int (*)(void*, void*))(numcmp),
						reverse);
				break;
			case fold:
				_qsort((void **) lineptr, 0, nlines-1,
						(int (*)(void*, void*))(fldcse),
						reverse);
				break;
		}
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
static int readlines(char *lineptr[], size_t maxlines)
{
	size_t len, nlines;
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
static int getline(char *s, size_t lim)
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
static char *alloc(size_t n)	/* return pointer to  characters */
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
static void writelines(char *lineptr[], size_t nlines)
{
	while (nlines-- > 0)
		printf("%s\n", *lineptr++);
}

/*
 * Sort v[left]...v[right] into increasing order.
 */
static void _qsort(void *v[], int left, int right,
		int (*comp)(void *, void *), int reversed)
{
	size_t i, last;

	if (left >= right)		/* do nothing if array cantains */
		return;			/* fewer than two eliments */

	swap(v, left, (left + right)/2);
	last = left;

	if (!reversed) {			/* check the sort order */
		for (i = left+1; i <= right; i++)
			if ((*comp)(v[i], v[left]) < 0)
				swap(v, ++last, i);
	} else {
		for (i = left+1; i <= right; i++)
			if ((*comp)(v[i], v[left]) > 0)
				swap(v, ++last, i);
	}

	swap(v, left, last);
	_qsort(v, left, last-1, comp, reversed);
	_qsort(v, last+1, right, comp, reversed);
}

/*
 * Compaire s1 and s2 alphabeticaly.
 */
static int fldcse(char *s1, char *s2)
{
        for ( ; tolower(*s1) == tolower(*s2); s1++, s2++)
                if (*s1 == '\0')
                        return 0;
        return tolower(*s1) - tolower(*s2);
}

/*
 * Compaire s1 and s2 numericaly.
 */
static int numcmp(char *s1, char *s2)
{
	double v1, v2;

	v1 = atof(s1);
	v2 = atof(s2);
	if (v1 < v2)
		return -1;
	else if (v1 > v2)
		return 1;
	else
		return 0;
}

/*
 * Interchange v[i] and v[j]
 */
static void swap(void *v[], size_t i, size_t j)
{
	void *temp;

	temp = v[i];
	v[i] = v[j];
	v[j] = temp;
}

