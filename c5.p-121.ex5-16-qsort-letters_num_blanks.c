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

static int readlines(char *lineptr[], size_t maxlines);
static int getline(char *, size_t);
static char *alloc(size_t);
static void writelines(char *lineptr[], size_t nlines);
static void _qsort(void *lineptr[], int left, int right, comp fn, int direction);

/* Functions for function pointers */
static long int strsrt(char *s1, char *s2);
static long int fldcse(char *s1, char *s2);
static long int dircse(char *s1, char *s2);

/* Function pointers */
static comp strings = (int (*)(void*, void*)) strsrt;
static comp strfold = (int (*)(void*, void*)) fldcse;
static comp directc = (int (*)(void*, void*)) dircse;

/* Memory */
static char *lineptr[MAXLINES];			/* Pointer to text lines */
static char allocbuf[ALLOCSIZE];		/* storage for alloc */
static char *allocp = allocbuf;			/* next free position */

enum function { alpha, fold, directory, nosort };
enum boolean { false, true };

/* Global flags */
#define DEBUG		false
static int numeric = 	false;

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
				case 'd':
					func = directory;
					break;
				case 'n':
					numeric = true;
					break;
				case 'r':
					reverse = true;
					break;
				case 'f':
					func = fold;
					break;
				case 'p':
					func = nosort;
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
				_qsort((void **) lineptr, 0,
						nlines-1, strings, reverse);
				break;
			case fold:
				_qsort((void **) lineptr, 0,
						nlines-1, strfold, reverse);
				break;
			case directory:
				_qsort((void **) lineptr, 0,
						nlines-1, directc, reverse);
				break;
			case nosort:
				break;
			default:
				break;
		}
		writelines(lineptr, nlines);
		return 0;
	} else {
		printf("Error: input to large\n");
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
static void _qsort(void *v[], int left, int right, comp fn, int reversed)
{
	size_t i, last;

	if (left >= right)		/* do nothing if array cantains */
		return;			/* fewer than two eliments */

	swap(v, left, (left + right)/2);
	last = left;

	if (!reversed) {		/* check the sort order */
		for (i = left+1; i <= right; i++)
			if ((*fn)(v[i], v[left]) < 0)
				swap(v, ++last, i);
	} else {
		for (i = left+1; i <= right; i++)
			if ((*fn)(v[i], v[left]) > 0)
				swap(v, ++last, i);
	}

	swap(v, left, last);
	_qsort(v, left, last-1, fn, reversed);
	_qsort(v, last+1, right, fn, reversed);
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Buffer for reading numerical values.
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
#define BUFSIZE		100
#define DECIMAL 	INT_MAX

static char buf[BUFSIZE];	/* Buffer for next ungetch */
static int bufp = 0;		/* next free position in buf */

static void getnumber(int c)	/* push character back on input */
{
	if (bufp >= BUFSIZE)
		printf("getnumber: too many characters\n");
	else {
		buf[bufp++] = c;
		buf[bufp] = '\0';
	}
}

/*
 * If the string starts with a number, read it and convert to a float.
 */
static double readnumber(char *n)
{
	double num;

	getnumber(*n++);
	while (isdigit(*n) || *n == '.')
		getnumber(*n++);
	num = atof(buf);
	bufp = 0;

	return num;
}

/*
 * Check which is largest number, deal with floatingpoint by seperating and
 * multiplying the decimal part by 100 and then changing it to an int.
 */
static long int numcheck(const double in1, const double in2)
{
	long int i1 = (long int) in1;
	long int i2 = (long int) in2;
	long int d1 = (long int) ((in1 - i1) * DECIMAL);
	long int d2 = (long int) ((in2 - i2) * DECIMAL);

	if (i1 == i2)
		return d1 - d2;

	return i1 - i2;
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

static int sortAlphaCase(char *c)
{
	int chars[] = {
		'\0','0','1','2','3','4','5','6','7','8','9',
		'a','A','b','B','c','C','d','D','e','E','f','F','g',
		'G','h','H','i','I','j','J','k','K','l','L','m','M',
		'n','N','o','O','p','P','q','Q','r','R','s','S','t',
		'T','u','U','v','V','w','W','x','X','y','Y','z','Z'
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
static long int strsrt(char *s1, char *s2)
{
	long int res;
	int b1, b2;
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
			num1 = readnumber(s1), b1 = true;

	/*
	 * Test s2.
	 */
	while (!isalnum(*s2) && *s2 != '\0')
		*s2++;
	if (numeric)
		if (isdigit(*s2))
			num2 = readnumber(s2), b2 = true;

	/*
	 * Return either alpahbetical or numerical order.
	 */
	if (b1 && b2)
		return numcheck(num1, num2);
	else {
		res = sortAlpha(s1) - sortAlpha(s2);
		if (!res && *s1 != '\0')
			res = strsrt(++s1, ++s2);
	}

        return res;
}

/*
 * Compaire s1 and s2 alphabeticaly.
 */
static long int fldcse(char *s1, char *s2)
{
	long int res;
	int b1, b2;
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
			num1 = readnumber(s1), b1 = true;

	/*
	 * Test s2.
	 */
	while (!isalnum(*s2) && *s2 != '\0')
		*s2++;
	if (numeric)
		if (isdigit(*s2))
			num2 = readnumber(s2), b2 = true;

	/*
	 * Return either alpahbetical or numerical order.
	 */
	if (b1 && b2)
		return numcheck(num1, num2);
	else {
		res = sortAlphaCase(s1) - sortAlphaCase(s2);
		if (!res && *s1 != '\0')
			res = fldcse(++s1, ++s2);
	}

        return res;
}

/*
 * Compaire s1 and s2 alphabeticaly.
 */
static long int dircse(char *s1, char *s2)
{
	long int res;
	int b1, b2;
	double num1, num2;

	res = b1 = b2 = 0;
	num1 = num2 = 0.0;

	/*
	 * Test s1.
	 */
	while (!isalnum(*s1) && *s1 != '\0')
		*s1++;
	if (isdigit(*s1))
		num1 = readnumber(s1), b1 = true;

	/*
	 * Test s2.
	 */
	while (!isalnum(*s2) && *s2 != '\0')
		*s2++;
	if (isdigit(*s2))
		num2 = readnumber(s2), b2 = true;

	/*
	 * Return either alpahbetical or numerical order.
	 */
	if (b1 && b2)
		return numcheck(num1, num2);
	else {
		res = sortAlphaCase(s1) - sortAlphaCase(s2);
		if (!res && *s1 != '\0')
			res = fldcse(++s1, ++s2);
	}

        return res;
}

