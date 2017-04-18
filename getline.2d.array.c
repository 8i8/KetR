#include <stdio.h>

#define MAXLINES	500			/* Maxlines to be sorted */
#define MAXLEN		1000			/* max length of any input line */
#define ALLOCSIZE	1000			/* size of available space */

/*
 * Input from stdin line by line.
 */
int _getline(char *s, int nlines, int lim)
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

int main(void)
{
	char allocbuf[ALLOCSIZE][MAXLEN];
	char* allocp;
	int nlines;

	allocp = (char*)allocbuf;

	nlines = 0;

	while (nlines < MAXLINES && !feof(stdin))
	{
		_getline(allocp, nlines, MAXLEN);
		printf("%d -> %s", nlines, &allocbuf[nlines][0]);
		nlines++;
	}

	return 0;
}

