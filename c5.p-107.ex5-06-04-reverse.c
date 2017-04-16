/*
 * Exercise 5-6. Rewrite appropriate programs from earlier chapters and
 * exercises with pointers instead of array indexing. Good possibilities
 * include getline (Chapters 1 and 4), atoi, itoa, and their variants (Chapters
 * 2, 3, and 4), reverse (Chapter 3), and strindex and getop (Chapter 4).
 *
 * reverse
 */
#include <stdio.h>
#include <string.h>

/*
 * Reverse the array using pointer arithmetic.
 */
static char* reverse(char *s)
{
	size_t i, j;
	char *s_in;
	int c;
	s_in = s;

	for (i = 0, j = strlen(s) - 1; i < j; i++, j--) {
		c = *(s+j);
		*(s+j) = *(s+i);
		*(s+i) = c;
	}
	return s_in;
}

int main(void)
{
	char test[] = "tseT";
	char *t;

	t = reverse(test);

	printf("%s", t);

	return 0;

}
