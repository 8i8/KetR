/*
 * Exercise 4-13. Write a recursive version of the function reverse(s), which
 * reverses the string s in place.
 */
#include <stdio.h>
#include <string.h>

/*
 * Function to revese a string using recursion.
 */
static void reverse(char s[])
{
	static size_t i, j = 0;
	char c;

	if (i == 0)
		j = strlen(s);

	c = s[--j];
	s[j] = s[i];
	s[i++] = c;

	if (i < j)
		reverse(s);
}

int main(void)
{
	char s[] = { "Hello" };

	reverse(s);
	printf("%s\n", s);

	return 0;
}

