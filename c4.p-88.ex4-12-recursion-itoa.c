/*
 * Exercise 4-12. Adapt the ideas of printd to write a recursive version of
 * itoa; that is, convert an integer into a string by calling a recursive
 * routine.
 */
#include <stdio.h>
#include <stdlib.h>

/*
 * Int to string function using recursion.
 */
void itoa(int n, char s[])
{
	static size_t i = 0;

	if (n < 0 && i == 0) {
		s[i++] = '-';
	}
	if (n / 10)
		itoa(n / 10, s);
	s[i++] = abs(n % 10) + '0';
	s[i] = '\0';
}

int main(void)
{
	char s[10];
	int n = 120;

	itoa(n, s);

	printf("%s", s);

	return 0;
}

