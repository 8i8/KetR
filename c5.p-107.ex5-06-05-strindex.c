/*
 * Exercise 5-6. Rewrite appropriate programs from earlier chapters and
 * exercises with pointers instead of array indexing. Good possibilities
 * include getline (Chapters 1 and 4), atoi, itoa, and their variants (Chapters
 * 2, 3, and 4), reverse (Chapter 3), and strindex and getop (Chapter 4).
 *
 * reverse
 */
#include <stdio.h>

/*
 * Return index of t in s, -1 if none.
 */
static int strindex(char *s, char *t)
{
	int j;
	char *s_in;
	s_in = s;

	while (*s) {
		j = 0;
		while (*(s+j) == *(t+j) && *(t+j) != '\0')
			j++;
		if (j > 0 && *(t+j) == '\0')
			return s - s_in + 1;
		*s++;
	}
	return -1;
}

int main(void)
{
	char s[] = "Hello World";
	char t[] = "World";
	int p;

	p = strindex(s, t);
	printf("%d\n", p);

	return 0;
}

