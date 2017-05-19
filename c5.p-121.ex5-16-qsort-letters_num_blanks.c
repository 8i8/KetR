/*
 * Exercise 5-16. Add the -d ("directory order") option, which makes
 * comparisons only on letters, numbers and blanks. Make sure it works in
 * conjunction with -f.
 */

/* Redefine getline */
#undef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#define MAXLEN		1000			/* max length of input line */
#define MAXLINES	5000			/* Maxlines to be sorted */
#define ALLOCSIZE	5000000			/* size of available space */

typedef int (*comp)(void *, void *);		/* Sort functions for qsort */
typedef short int bool;

static int readlines(char *lineptr[], size_t maxlines);
static int getline(char *, size_t);
static char *alloc(size_t);
static void writelines(char *lineptr[], size_t nlines);
static void _qsort(void *lineptr[], int left, int right, comp fn);

/* Sort functions */
static int sortAlpha(char *s1, char *s2);
static int sortAlphaCase(char *s1, char *s2);
static int sortNumeric(char *s1, char *s2);

/* Function pointers */
static comp strings = (int (*)(void*, void*)) sortAlpha;
static comp strfold = (int (*)(void*, void*)) sortAlphaCase;
static comp numsort = (int (*)(void*, void*)) sortNumeric;

/* Global Memory */
static char *lineptr[MAXLINES];			/* Pointer to text lines */
static char allocbuf[ALLOCSIZE];		/* storage for alloc */
static char *allocp = allocbuf;			/* next free position */

enum function { alpha, fold, number, nosort };
enum boolean { false, true };

/* Global flags */
#define DEBUG		false
static int numeric = 	false;			/* numeric sort ectra cycles */
static int reverse = 	false;			/* reverse search order */

/*
 * Sort input lines.
 */
int main(int argc, char *argv[])
{
	int nlines;		/* number of input lines to read */
	int func = alpha;
	int c;

	while (--argc > 0 && (*++argv)[0] == '-')
		while ((c = *++argv[0]))
			switch (c) {
				case 'n':
					numeric = true;
					func = number;
					break;
				case 'r':
					reverse = true;
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
				_qsort((void **) lineptr, 0, nlines-1, strings);
				break;
			case fold:
				_qsort((void **) lineptr, 0, nlines-1, strfold);
				break;
			case number:
				_qsort((void **) lineptr, 0, nlines-1, numsort);
				break;
			case nosort:
				break;
			default:
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

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Compaire and sort.
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */

static void swap(void *v[], size_t i, size_t j);
static long int rsort(char *left, char *right, comp fn);
static int numcmp(char *s1, char *s2);

/*
 * Sort v[left]...v[right] into increasing order.
 */
static void _qsort(void *v[], int left, int right, comp fn)
{
	size_t i, last;

	if (left >= right)	/* do nothing if array contains */
		return;		/* fewer than two elements */

	swap(v, left, (left + right)/2);
	last = left;

	if (!reverse) {
		for (i = left+1; i <= right; i++)
			if (rsort(v[i], v[left], fn) < 0)
				swap(v, ++last, i);
	} else
		for (i = left+1; i <= right; i++)
			if (rsort(v[i], v[left], fn) > 0)
				swap(v, ++last, i);

	swap(v, left, last);
	_qsort(v, left, last-1, fn);
	_qsort(v, last+1, right, fn);
}

/*
 * Prepare string for sort function, filter numbers and letters and allow for
 * recursive call to sort function provided to _qsort(); seperating this
 * section of the function allows for the reverse '-r' functionality.
 */
static long int rsort(char *left, char *right, comp fn)
{
	long int res;
	bool b1, b2;

	res = b1 = b2 = 0;

	/*
	 * Test left.
	 */
	while (!isalnum(*left) && *left != '\0')
		*left++;
	if (numeric)
		if (isdigit(*left))
			b1 = true;
	/*
	 * Test right.
	 */
	while (!isalnum(*right) && *right != '\0')
		*right++;
	if (numeric)
		if (isdigit(*right))
			b2 = true;
	/*
	 * Return either alphabetical or numerical order.
	 */
	if (b1 && b2)
		res = numcmp(left, right);
	else {
		res = (*fn)(left, right);
		if (!res && *left != '\0')
			res = rsort(++left, ++right, fn);
	}

        return res;
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

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Sort maps.
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */

static int getposit(int *chars, char *c)
{
	int *in_pt;
	in_pt = chars;

	while (*chars++ != (int)*c)
		;

	return --chars - in_pt;
}

/*
 * Sorting character maps.
 */
static int sortAlpha(char *s1, char *s2)
{
	int chars[] = {
		'\0','0','1','2','3','4','5','6','7','8','9',
		'a','b','c','d','e','f','g','h','i','j','k','l','m',
		'n','o','p','q','r','s','t','u','v','w','x','y','z',
		'A','B','C','D','E','F','G','H','I','J','K','L','M',
		'N','O','P','Q','R','S','T','U','V','W','X','Y','Z'
	};

	return getposit(chars, s1) - getposit(chars, s2);
}

static int sortAlphaCase(char *s1, char *s2)
{
	int chars[] = {
		'\0',
		'a','A','b','B','c','C','d','D','e','E','f','F','g',
		'G','h','H','i','I','j','J','k','K','l','L','m','M',
		'n','N','o','O','p','P','q','Q','r','R','s','S','t',
		'T','u','U','v','V','w','W','x','X','y','Y','z','Z',
		'0','1','2','3','4','5','6','7','8','9'
	};

	return getposit(chars, s1) - getposit(chars, s2);
}

static int sortNumeric(char *s1, char *s2)
{
	int chars[] = {
		'\0',
		'a','b','c','d','e','f','g','h','i','j','k','l','m',
		'n','o','p','q','r','s','t','u','v','w','x','y','z',
		'A','B','C','D','E','F','G','H','I','J','K','L','M',
		'N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
		'0','1','2','3','4','5','6','7','8','9'
	};

	return getposit(chars, s1) - getposit(chars, s2);
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

