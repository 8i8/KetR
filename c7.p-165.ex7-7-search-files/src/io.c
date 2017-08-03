/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  i/o
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
#include "search-files.h"

static size_t getline(char *, size_t);
static char *alloc(size_t);

/*
 * Read input lines, check available space for new line, return line count.
 * Copy the new line into the allocated space and fill lineptr array with
 * pointers to the new lines gathered.
 */
size_t readlines(char *lineptr[], size_t maxlines)
{
	size_t len, nlines;
	char *p, line[MAXLEN];

	nlines = 0;
	while ((len = getline(line, MAXLEN)) > 0)
		if (nlines >= maxlines || (p = alloc(len)) == NULL)
			return 0;
		else {
			if (state.remempty && len == 1)
				continue;
			line[len-1] = '\0'; /* delete newline char*/
			strcpy(p, line);
			lineptr[nlines++] = p;
		}

	return nlines;
}

void getargs(char* argument, char* name, size_t file)
{
	/* Input files if address given */
	else if ((fp = fopen(argument, "r")) != NULL)
		folio[file].fp = fp, folio[file].name = name;
	else
		folio[file].fp = NULL, folio[file].name = name;
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
 * Add new line to char* array, after given index value.
 */
size_t insertline(char *lineptr[], char* line, size_t maxlines, size_t index, size_t nlines)
{
	char *p;
	size_t i = 0;

	/* If there is room in alloc buffer ... */
	if (nlines >= maxlines || (p = alloc(strlen(line)+1)) == NULL) {
		printf("Error alloc: insuficiant place in allocbuf[]\n");
		return 0;
	}

	/* Copy the above line to p */
	strcpy(p, line);
	nlines++;

	/* Add a new index space shunting all pointers forwards one place. */
	i = nlines;
	while (--i > index)
		lineptr[i] = lineptr[i-1];
 
	 /* Add p to the newly created index space. */
	lineptr[index] = p; 

	return nlines;
}

/*
 * Count memory use for assigning input, first check that the requested length
 * is available, if it is return a pointer to the array place, if not return 0.
 */
static char *alloc(size_t n)	/* return pointer to  characters */
{
	if (allocbuf + ALLOCSIZE - allocp >= (int)n) { /* if 'n' fits */
		allocp += n;
		return allocp - n;	/* old p */
	} else		/* not enough room */
		return 0;
}

/*
 * Remove line from allocbuf.
 */
static int freealloc(char *allocbuf)
{
	int len;
	char* line;
	line = allocbuf;

	len = strlen(line)+1;

	if (len) {
		allocp -= len;
		while (line < allocp) {
			*line = *(line+len);
			line++;
		}
		return len;
	} else
		printf("Error freealloc: no string provided\n");

	return 1;
}

/*
 * Delete line.
 */
size_t deleteline(char *lineptr[], int line, size_t nlines)
{
	size_t i = 0;
	int len;
	char *m;

	m = lineptr[line];

	if ((len = freealloc(lineptr[line]))) {
		/*
		 * Displace all pointers after the removed by one place.
		 */
		while (line < (int)nlines) {
			lineptr[line] = lineptr[line+1];
			line++;
		}
		/*
		 * Diminish any address that is greater than the address of
		 * the removed line by the lenght of the remove line.
		 */
		while (i < nlines) {
			if (lineptr[i] > m)
				lineptr[i] -= len;
			i++;
		}
		return --nlines;
	} else
		printf("Error deleteline: freealloc failed\n");

	return nlines;
}

/*
 * Set system tab width.
 */
void settabs(char n[])
{
	char tabs[10] = { "tabs " };
	strcat(tabs, n);
	system(tabs);
}

/*
 * Write output.
 */
void writelines(char *lineptr[], size_t nlines)
{
	size_t i = 0;

	while (nlines-- > 0)
		if (state.linenum)
			printf("%3lu: %s\n", i++, *lineptr++);
		else
			printf("%s\n", *lineptr++);
}

