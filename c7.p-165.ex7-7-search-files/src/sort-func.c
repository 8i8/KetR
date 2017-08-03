/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Sort maps and tools.
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
#include "search-files.h"

/*
 * Interchange v[i] and v[j]
 */
void swap(void *v[], size_t i, size_t j)
{
	void *temp;

	temp = v[i];
	v[i] = v[j];
	v[j] = temp;
}

/*
 * Skip over all spaces and non alphanumeric char from the start of the string.
 */
char* jumptochar(char *c)
{
	while (!isalnum(*c) && *c != '\0' && *c != '\t')
		c++;
	return c;
}

/*
 * Skip to the n'th tab.
 */
char* jumptotab(char *c, int ntab)
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
int sortAlpha(char *s1, char *s2)
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
int sortAlphaCase(char *s1, char *s2)
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
int numcmp(char *s1, char *s2)
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

int strtcmp(char *s, char *t)
{
        for ( ; *s == *t; s++, t++)
                if (*s == '\0' || *s == '\t')
                        return 0;
        return *s - *t;
}

