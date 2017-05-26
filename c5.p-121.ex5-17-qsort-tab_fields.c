/*
 * Exercise 5-17. Add a field-searching capability, so sorting may be done on
 * fields within lines, each field sorted according to an independent set of
 * options. (The index for this book was sorted with -df for the index category
 * and -n for the page numbers.)
 *
 * input arguments:
 *
 * 	-d, directory sort
 * 	-e, remove blank lines
 *	-f, fold lower case to upper case characters
 *	-n, numerical sort
 *	-p, no sort
 *	-r, reverse sort
 *	-s, basic string compare
 */

/* Redefine getline */
#undef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define MAXLEN		1000			/* Max length of input line */
#define MAXLINES	5000			/* Lines to be sorted */
#define ALLOCSIZE	5000000			/* Size of available space */
#define MAXTOKEN	6			/* Maximum number of tokens */
#define TOKENSIZE	24			/* Size of space for input tokens */

typedef int (*comp)(void *, void *);		/* Sort functions for qsort */
typedef short int bool;

static void settings(int argc, char*argv[]);
static void settabs(char n[]);
static size_t readlines(char *lineptr[], size_t maxlines);
static size_t getline(char *, size_t);
static char *alloc(size_t);
static void writelines(char *lineptr[], size_t nlines);
/* */
static void sortsection(char *lineptr[], int left, int right, int func, int ntab);
static size_t sortdivide(char *lineptr[], int func, size_t nlines, int ntab);
/* */
static void _qsort(void *lineptr[], int left, int right, comp fn, int ntab);
static size_t addspacer(char *lineptr[], size_t maxlines, size_t nlines, int ntab);
static int firstcmp(char *s1, char *s2, int ntab);

/* Sort functions */
static int sortAlpha(char *s1, char *s2);
static int sortAlphaCase(char *s1, char *s2);

/* Function pointers */
static comp strsimp = (int (*)(void*, void*)) strcmp;
static comp stnsort = (int (*)(void*, void*)) sortAlpha;
static comp strfold = (int (*)(void*, void*)) sortAlphaCase;

/* Global Memory */
static char *lineptr[MAXLINES];			/* Pointer to text lines */
static char allocbuf[ALLOCSIZE];		/* Storage for alloc */
static char *allocp = allocbuf;			/* Next free position */

enum function { simple, alpha, fold, number, nosort };
enum boolean { false, true };

/* Global flags */
#define DEBUG		false
static bool numeric = 	false;			/* use numeric sort in qsort */
static bool reverse = 	false;			/* reverse search order */
static bool remempty =	false;
static bool directory =	false;
static bool resort = 	false;
static int func = alpha;

/*
 * Sort input lines.
 */
int main(int argc, char *argv[])
{
	size_t nlines;		/* number of input lines to read */
	int i;

	/* Set system tab size */
	settabs("24");

	/* If there are arguments entered, read argv[1] */
	i = 1;
	if (argc > i)
		settings(i, argv);

	/*
	 * Fill lineptr array from stdin.
	 */
	if (!(nlines = readlines(lineptr, MAXLINES))) {
		printf("Error: input to big to sort\n");
		return 1;
	}
	
	/* Sort input */
	sortsection(lineptr, 0, nlines-1, func, 0);
	
	if (directory)
		nlines = addspacer(lineptr, MAXLINES, nlines, 0);

	/* Sort using argv */
	while (++i < argc) {
		settings(i, argv);

		/* Sort input */
		if (resort)
			sortsection(lineptr, 0, nlines-1, func, i-1);

		sortdivide(lineptr, func, nlines, i-1);

		/*
		 * If directory setting is used, add a blank line break
		 * after each new starting letter.
		 */
		if (directory)
			nlines = addspacer(lineptr, MAXLINES, nlines, i-1);
	}

	/*
	 * Output to terminal.
	 */
	writelines(lineptr, nlines);

	/* Set system default tab size */
	settabs("-8");

	return 0;
}

/*
 * Set state before running qsort.
 */
static void settings(int argc, char*argv[])
{
	int c;

	if (DEBUG) printf("argc %d argv --> -", argc);

	numeric = 	false;
	reverse = 	false;
	remempty =	false;
	directory =	false;
	resort = 	false;

	if (*argv[argc] == '-')
		while ((c = *++argv[argc])) {
			if (DEBUG) printf("%c", *argv[argc]);
			switch (c) {
				case 'a':
					func = alpha;
					break;
				case 'd':
					directory = true;
					break;
				case 'e':
					remempty = true;
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
				case 'R':
					resort = true;
				default:
					break;
			}
		}
	if (DEBUG) putchar('\n');
}

/*
 * Switch, selects the sort function for qsort.
 */
static void sortsection(char *lineptr[], int left, int right, int func, int ntab)
{
	switch (func) {
		case simple:
			_qsort((void **) lineptr, left, right, strsimp, ntab);
			break;
		case alpha:
			_qsort((void **) lineptr, left, right, stnsort, ntab);
			break;
		case fold:
			_qsort((void **) lineptr, left, right, strfold, ntab);
			break;
		case nosort:
			break;
		default:
			break;
	}
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  io
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */

/*
 * Read input lines, check available space for new line, return line count.
 * Copy the new line into the allocated space and fill lineptr array with
 * pointers to the new lines gathered.
 */
static size_t readlines(char *lineptr[], size_t maxlines)
{
	size_t len, nlines;
	char *p, line[MAXLEN];

	nlines = 0;
	while ((len = getline(line, MAXLEN)) > 0)
		if (nlines >= maxlines || (p = alloc(len)) == NULL)
			return 0;
		else {
			if (remempty && len == 1)
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
static size_t getline(char *s, size_t lim)
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
 * Add newly created empty line to char* array, after given index value.
 */
static size_t insertline(char *lineptr[], size_t maxlines, size_t index, size_t nlines)
{
	char line[1] = { '\0' };
	char *p;
	size_t i = 0;

	/* If there is room in alloc buffer ... */
	if (nlines >= maxlines || (p = alloc(1)) == NULL)
		return -1;

	/* Copy the above line to p */
	strcpy(p, line);
	nlines++;

	/* Add a new index space shunting all pointers forwards one place. */
	i = nlines;
	while (--i > index)
		lineptr[i] = lineptr[i-1];
 
	 /* Add p to the newly created index space. */
	lineptr[i] = p; 

	return nlines;
}

/*
 * Set system tab width.
 */
static void settabs(char n[])
{
	char tabs[10] = { "tabs " };
	strcat(tabs, n);
	system(tabs);
}

/*
 * Write output.
 */
static void writelines(char *lineptr[], size_t nlines)
{
	while (nlines-- > 0)
		printf("%s\n", *lineptr++);
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Sort.
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */

static void swap(void *v[], size_t i, size_t j);
static int nsort(char *left, char *right, comp fn, int ntab);
static int numcmp(char *s1, char *s2);
static char* jumptochar(char *c);
static char* jumptotab(char *c, int ntab);

/*
 * Sort v[left]...v[right] into increasing order.
 */
static void _qsort(void *v[], int left, int right, comp fn, int ntab)
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
			if (nsort(v[i], v[left], fn, ntab) < 0)
				swap(v, ++last, i);
	} else
		for (i = left+1; i <= right; i++)
			if (nsort(v[i], v[left], fn, ntab) > 0)
				swap(v, ++last, i);

	swap(v, left, last);
	_qsort(v, left, last-1, fn, ntab);
	_qsort(v, last+1, right, fn, ntab);
}

/*
 * Prepare string for sort function, filter numbers and letters, recursive call
 * to sort function; By separating this section of the function from the body
 * of qsort, enabling shorter reverse '-r' code in qsort.
 */
static int nsort(char *left, char *right, comp fn, int ntab)
{
	char *l_pt, *r_pt;
	int res = 0;
	bool b1, b2, p1, p2;
	b1 = b2 = p1 = p2 = false;
	l_pt = left, r_pt = right;

	/*
	 * Move to desired tab.
	 */
	if (ntab) {
		if ((left = jumptotab(left, ntab)) == NULL)
			left = l_pt, p1 = true;
		if ((right = jumptotab(right, ntab)) == NULL)
			right = r_pt, p2 = true;
		/*
		 * If either pointers returns null, return 0, the values are
		 * not to be swapped.
		 */
		if (p1 == true || p2 == true)
			return 0;
	}

	/*
	 * Remove redundant char.
	 */
	left = jumptochar(left);
	right = jumptochar(right);

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
		if (!res && (*left != '\0' || *left != '\t'))
			res = nsort(++left, ++right, fn, ntab);
	} else {
		res = (*fn)(left, right);
		if (!res && (*left != '\0' || *left != '\t'))
			res = nsort(++left, ++right, fn, ntab);
	}

        return res;
}

/*
 * Compare the first char of each line, return 0 if there is an alphabetical
 * match and 1 if there is a difference. Essentially to select the input scope
 * of qsort, left and right. Also used when adding empty spacer lines.
 */
static int firstcmp(char *s1, char *s2, int ntab)
{
	bool p1, p2;
	char *s1_pt, *s2_pt;
	p1 = p2 = false;
	s1_pt = s1, s2_pt = s2;

	/*
	 * Jump to specified tab if it exists in both strings, else return 0;
	 */
	if (ntab) {
		if ((s1 = jumptotab(s1, ntab)) == NULL)
			s1 = s1_pt, p1 = true;
		if ((s2 = jumptotab(s2, ntab)) == NULL)
			s2 = s2_pt, p2 = true;
		/*
		 * If either pointers return null the tab does not exist,
		 * return 1, values are not the same.
		 */
		if (p1 == true || p2 == true)
			return 1;
	}

	/*
	 * Jump to the first relevant character.
	 */
	s1 = jumptochar(s1);
	s2 = jumptochar(s2);

	/*
	 * If the char differ return 0, else return 1 should they be the same.
	 */
	if (sortAlphaCase(s1, s2) && (isalpha(*s1) || isalpha(*s2)))
		return 0;
	else if (isdigit(*s1) && isdigit(*s2))
		if (*s1 != *s2)
			return 0;
	return 1;
}

/*
 * Search over the array looking for lines that are grouped together
 * alphabetically or as blocks of numbers, select the start and end index of
 * each group and then sort by the next argv input using the given tab field.
 */
static size_t sortdivide(char *lineptr[], int func, size_t nlines, int ntab)
{
	size_t i, j;
	i = j = 0;

	while (++i < nlines)
		/*
		 * If the first char of both lines differ, start the count.
		 */
		if (!firstcmp(lineptr[i-1], lineptr[i], ntab)) {
			j = i-1;
			/*
			 * Whilst the first char of the prior tab stop are the
			 * same; Keep on counting.
			 */
			while (i < nlines && firstcmp(lineptr[i-1], lineptr[i], 0))
				i++;
			/*
			 * Perform sort between this current change of letter
			 * and the last stored index j; then store i as j.
			 */
			sortsection(lineptr, j, i-1, func, ntab);
		}

	return nlines;
}

/*
 * Add empty 'spacer' line.
 */
static size_t addspacer(char *lineptr[], size_t maxlines, size_t nlines, int ntab)
{
	size_t i = 0;

	while (++i < nlines)
		if (!firstcmp(lineptr[i-1], lineptr[i], ntab) && 
				(!isdigit(*lineptr[i-1]) || !isdigit(*lineptr[i])))
			nlines = insertline(lineptr, maxlines, i++, nlines);

	return nlines;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Sort maps and tools.
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */

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
static char* jumptochar(char *c)
{
	while (!isalnum(*c) && *c != '\0' && *c != '\t')
		c++;
	return c;
}

/*
 * Skip to the n'th tab.
 */
static char* jumptotab(char *c, int ntab)
{
	while (*c != '\0')
		if (*c++ == '\t' && --ntab == 0)
			return c;

	return NULL;
}

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
	return 0;
}

