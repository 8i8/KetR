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

#define MAXLEN		1000			/* max length of input line */
#define MAXLINES	5000			/* Maxlines to be sorted */
#define ALLOCSIZE	5000000			/* size of available space */

typedef int (*comp)(void *, void *);		/* Sort functions for qsort */
typedef short int bool;

static int readlines(char *lineptr[], size_t maxlines, bool emptylines);
static int getline(char *, size_t);
static char *alloc(size_t);
static void writelines(char *lineptr[], size_t nlines);
static size_t addspaces(char *lineptr[], size_t maxlines, size_t nlines);
static void _qsort(void *lineptr[], int left, int right, comp fn);
static int firstcmp(char *s1, char *s2);

/* Sort functions */
static int sortAlpha(char *s1, char *s2);
static int sortAlphaCase(char *s1, char *s2);

/* Function pointers */
static comp strsimp = (int (*)(void*, void*)) strcmp;
static comp stnsort = (int (*)(void*, void*)) sortAlpha;
static comp strfold = (int (*)(void*, void*)) sortAlphaCase;

/* Global Memory */
static char *lineptr[MAXLINES];			/* Pointer to text lines */
static char allocbuf[ALLOCSIZE];		/* storage for alloc */
static char *allocp = allocbuf;			/* next free position */

enum function { simple, alpha, fold, number, nosort };
enum boolean { false, true };

/* Global flags */
#define DEBUG		false
static bool numeric = 	false;			/* use numeric sort in qsort */
static bool reverse = 	false;			/* reverse search order */

/*
 * Sort input lines.
 */
int main(int argc, char *argv[])
{
	int nlines;		/* number of input lines to read */
	int func = alpha;
	int c;
	bool emptylines = true;
	bool directory = false;

	while (--argc > 0 && (*++argv)[0] == '-')
		while ((c = *++argv[0]))
			switch (c) {
				case 'd':
					emptylines = false;
					directory = true;
					break;
				case 'e':
					emptylines = false;
					break;
				case 'f':
					func = fold;
					break;
				case 'n':
					numeric = true;
					break;
				case 'p':
					func = nosort;
					break;
				case 'r':
					reverse = true;
					break;
				case 's':
					func = simple;
					break;
				default:
					break;
			}
			
	/*
	 * Run qsort with appropriate function and settings.
	 */
	if ((nlines = readlines(lineptr, MAXLINES, emptylines)) >= 0)
		switch (func) {
			case simple:
				_qsort((void **) lineptr, 0, nlines-1, strsimp);
				break;
			case alpha:
				_qsort((void **) lineptr, 0, nlines-1, stnsort);
				break;
			case fold:
				_qsort((void **) lineptr, 0, nlines-1, strfold);
				break;
			case nosort:
				break;
			default:
				break;
		}
	else {
		printf("Error: input to big to sort\n");
		return 1;
	}

	/*
	 * If directory setting is used, add a blank line break after each new
	 * starting letter.
	 */
	if (directory)
		nlines = addspaces(lineptr, MAXLINES, nlines);

	/*
	 * Output to terminal.
	 */
	writelines(lineptr, nlines);

	return 0;
}

/*
 * Read input lines, check available space for new line, return line count.
 * Copy the new line into the allocated space and fill lineptr array with
 * pointers to the new lines gathered.
 */
static int readlines(char *lineptr[], size_t maxlines, bool emptylines)
{
	size_t len, nlines;
	char *p, line[MAXLEN];

	nlines = 0;
	while ((len = getline(line, MAXLEN)) > 0)
		if (nlines >= maxlines || (p = alloc(len)) == NULL)
			return -1;
		else {
			if (!emptylines && len == 1)
				continue;
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
 * Add empty line to char* array, after given index value.
 */
static size_t insertline(char *lineptr[], size_t maxlines, size_t index, size_t nlines)
{
	char line[1] = { '\0' };
	char *p;
	size_t i = 0;

	if (nlines >= maxlines || (p = alloc(1)) == NULL)
		return -1;

	strcpy(p, line);
	nlines++;

	i = nlines;

	while (--i > index)
		lineptr[i] = lineptr[i-1];

	lineptr[++index] = p; 

	return nlines;
}

/*
 * Add line when sorted and first letter of the line changes.
 */
static size_t addspaces(char *lineptr[], size_t maxlines, size_t nlines)
{
	size_t i = 1;

	for (; i < nlines; i++)
		if (firstcmp(lineptr[i-1], lineptr[i]))
			nlines = insertline(lineptr, maxlines, i-1, nlines), i++;

	return nlines;
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

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Compare and sort.
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */

static void swap(void *v[], size_t i, size_t j);
static int nsort(char *left, char *right, comp fn);
static int numcmp(char *s1, char *s2);
static char* remchar(char *c);

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

	/*
	 * Perform sort in either the direct or the reverse order.
	 */
	if (!reverse) {
		for (i = left+1; i <= right; i++)
			if (nsort(v[i], v[left], fn) < 0)
				swap(v, ++last, i);
	} else
		for (i = left+1; i <= right; i++)
			if (nsort(v[i], v[left], fn) > 0)
				swap(v, ++last, i);

	swap(v, left, last);
	_qsort(v, left, last-1, fn);
	_qsort(v, last+1, right, fn);
}

/*
 * Prepare string for sort function, filter numbers and letters and allow for
 * recursive call to sort function provided to _qsort(); separating this
 * section of the function allows for the reverse '-r' functionality.
 */
static int nsort(char *left, char *right, comp fn)
{
	int res = 0;
	bool b1, b2;
	b1 = b2 = false;

	/*
	 * Remove redundant char.
	 */
	left = remchar(left);
	right = remchar(right);

	if (numeric) {
		if (isdigit(*left))
			b1 = true;
		if (isdigit(*right))
			b2 = true;
	}

	/*
	 * Return either alphabetical or numerical order.
	 */
	if (b1 && b2) {
		res = numcmp(left, right);
		if (!res && *left != '\0')
			res = nsort(++left, ++right, fn);
	} else {
		res = (*fn)(left, right);
		if (!res && *left != '\0')
			res = nsort(++left, ++right, fn);
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

/*
 * Skip over all spaces and non alphanumeric char from the start of the string.
 */
static char* remchar(char *c)
{
	while (!isalnum(*c) && *c != '\0')
		*c++;
	return c;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Sort maps and comparisons.
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */

/*
 * Conversion used for sortAlphaCase.
 */
static int sortascii(int *c, bool fold)
{
	if (isupper(*c))
		if (fold)
			return *c = tolower(*c);
		else
			return *c += 57;
	else if (islower(*c))
		return *c;
	else if (isdigit(*c))
		return *c += 118;
	return 0;
}

/*
 * Sorting character maps.
 */
static int sortAlpha(char *s1, char *s2)
{
	int c1, c2;
	c1 = *s1, c2 = *s2;
	c1 = sortascii(&c1, false);
	c2 = sortascii(&c2, false);

	return c1 - c2;
}

/*
 * Sort string with Upper case folded in.
 */
static int sortAlphaCase(char *s1, char *s2)
{
	int c1, c2;
	c1 = *s1, c2 = *s2;
	c1 = sortascii(&c1, true);
	c2 = sortascii(&c2, true);

	return c1 - c2;
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

/*
 * Compare the first char of each line, used to separate alphabetically.
 */
static int firstcmp(char *s1, char *s2)
{
	s1 = remchar(s1);
	s2 = remchar(s2);
	if (sortAlphaCase(s1, s2) && (isalpha(*s1) || isalpha(*s2)))
		return 1;
	return 0;
}

