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

typedef short int bool;
enum boolean { false, true };

static int readlines(char *lineptr[], size_t maxlines);
static int getline(char *, size_t);
static char *alloc(size_t);
static void writelines(char *lineptr[], size_t nlines);
static void _qsort(void *lineptr[], int left, int right, int (*comp)(void *, void *));

static int strsrt(char *s1, char *s2);
static int numcmp(char *s1, char *s2);

static char *lineptr[MAXLINES];			/* Pointer to text lines */
static char allocbuf[ALLOCSIZE];		/* storage for alloc */
static char *allocp = allocbuf;			/* next free position */

static int numeric = 0;				/* 1 if numeric sort */
static int reverse = 0;				/* reverse search order */

/*
 * Sort input lines.
 */
int main(int argc, char *argv[])
{
	int nlines;		/* number of input lines to read */
	int c;

	while (--argc > 0 && (*++argv)[0] == '-')
		while ((c = *++argv[0]))
			switch (c) {
				case 'n':
					numeric = 1;
					break;
				case 'r':
					reverse = 1;
					break;
				default:
					break;
			}
			
	if ((nlines = readlines(lineptr, MAXLINES)) >= 0) {
		_qsort((void **) lineptr, 0, nlines-1,
				(int (*)(void*, void*))(strsrt));
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
 * Interchange v[i] and v[j]
 */
static void swap(void *v[], size_t i, size_t j)
{
	void *temp;

	temp = v[i];
	v[i] = v[j];
	v[j] = temp;
}

/*
 * Sort v[left]...v[right] into increasing order.
 */
static void _qsort(void *v[], int left, int right,
		int (*comp)(void *, void *))
{
	size_t i, last;

	if (left >= right)	/* do nothing if array contains */
		return;		/* fewer than two elements */

	swap(v, left, (left + right)/2);
	last = left;

	if (!reverse) {
		for (i = left+1; i <= right; i++)
			if ((*comp)(v[i], v[left]) < 0)
				swap(v, ++last, i);
	} else
		for (i = left+1; i <= right; i++)
			if ((*comp)(v[i], v[left]) > 0)
				swap(v, ++last, i);

	swap(v, left, last);
	_qsort(v, left, last-1, comp);
	_qsort(v, last+1, right, comp);
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Sort maps.
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */

/*
 * Sorting character maps.
 */
static int sortAlpha(char *c)
{
	int chars[] = {
		'\0','0','1','2','3','4','5','6','7','8','9',
		'a','b','c','d','e','f','g','h','i','j','k','l','m',
		'n','o','p','q','r','s','t','u','v','w','x','y','z',
		'A','B','C','D','E','F','G','H','I','J','K','L','M',
		'N','O','P','Q','R','S','T','U','V','W','X','Y','Z'
	};
	int *c_st = chars;
	int *c_pt = chars;

	while (*c_pt++ != (int)*c)
		;

	return c_pt - c_st;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Compaire.
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */

/*
 * Compaire s1 and s2 alphabeticaly.
 */
static int strsrt(char *s1, char *s2)
{
	int res;
	bool b1, b2;
	double num1, num2;

	res = num1 = b1 = num2 = b2 = 0;
	num1 = num2 = 0.0;

	/*
	 * Test s1.
	 */
	while (!isalnum(*s1) && *s1 != '\0')
		*s1++;
	if (numeric)
		if (isdigit(*s1))
			b1 = true;

	/*
	 * Test s2.
	 */
	while (!isalnum(*s2) && *s2 != '\0')
		*s2++;
	if (numeric)
		if (isdigit(*s2))
			b2 = true;

	/*
	 * Return either alphabetical or numerical order.
	 */
	if (b1 && b2)
		res = numcmp(s1, s2);
	else {
		res = sortAlpha(s1) - sortAlpha(s2);
		if (!res && *s1 != '\0')
			res = strsrt(++s1, ++s2);
	}

        return res;
}

/*
 * Compare s1 and s2 numerically.
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

