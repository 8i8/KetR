/*
 * Exercise 5-6. Rewrite appropriate programs from earlier chapters and
 * exercises with pointers instead of array indexing. Good possibilities
 * include getline (Chapters 1 and 4), atoi, itoa, and their variants (Chapters
 * 2, 3, and 4), reverse (Chapter 3), and strindex and getop (Chapter 4).
 *
 * atoa
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Reverse the array.
 */
static void __reverse(char s[])
{
	size_t i, j;
	int c;

	for (i = 0, j = strlen(s) - 1; i < j; i++, j--) {
		c = s[j];
		s[j] = s[i];
		s[i] = c;
	}
}

/*
 * Transform a value of the int type into a string.
 */ 
void itoa(int n, char *s)
{
	int sign;
	char *s_in;
	s_in = s;

	/* record the sign */
	sign = n;

	do {
		/* make n positive and generate digits in reverse order */
		*s++ = (char)abs(n % 10) + '0';
	}
	while ((n /= 10) != 0);

	/* Add the sign if it is required */
	if (sign < 0)
		*s++ = '-';

	*++s = '\0';

	__reverse(s_in);
}

int main(void)
{
	int i = 108;
	char s[12];

	itoa(i, s);

	printf("%s", s);

	return 0;
}
