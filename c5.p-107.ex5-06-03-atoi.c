/*
 * Exercise 5-6. Rewrite appropriate programs from earlier chapters and
 * exercises with pointers instead of array indexing. Good possibilities
 * include getline (Chapters 1 and 4), atoi, itoa, and their variants (Chapters
 * 2, 3, and 4), reverse (Chapter 3), and strindex and getop (Chapter 4).
 *
 * atoi
 */
#include <stdio.h>
#include <ctype.h>

/*
 * Convert char *s to an integer by way of a pointer.
 */
int atoi(char *s)
{
	int n, sign;
	n = 0;

	while (isspace(*s))
		*s++;
	sign = (*s == '-') ? -1 : 1;
	if (*s == '+' || *s == '-')
		*s++;
	while (isdigit(*s) && *s != '\n') {
		n = 10 * n + (*s++ - '0');
	}
	return sign * n;
}

int main(void)
{
	char s[] = "12345";
	int i;

	i = atoi(s);

	printf("%d", i);

	return 0;
}

