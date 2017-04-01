/*
 * Exercise 3-6. Write a version of itoa that accepts three arguments instead
 * of two. The third argument is a minimum field width; the converted number
 * must be padded with blanks on the left if necessary to make it wide enough.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <stdint.h>

#define MAXLEN	255

static void __reverse(char s1[]);
void itoa(int32_t n, char s[], uint8_t minWidth);

int main(void)
{
	char s[MAXLEN];
	int32_t n;
	n = 256;

	itoa(n, s, 5);
	printf("The value --> %s\n", s);

	return 0;
}

/*
 * Transform a value of the int type into a string.
 */ 
void itoa(int32_t n, char s[], uint8_t minWidth)
{
	size_t i, j;
	int8_t sign;

	/* record the sign */
	sign = n;

	i = j = 0;
	do {
		/* make n positive and generate digits in reverse order */
		s[i++] = (char)abs(n % 10) + '0';
	}
	while ((n /= 10) != 0);

	/* Add 0's to make up the minimum width */
	while(i < minWidth)
		s[i++] = '0';

	/* Add the sign if it is required */
	if (sign > 0)
		s[i++] = '-';

	s[i] = '\0';

	__reverse(s);
}

/*
 * Reverse the array.
 */
static void __reverse(char s1[])
{
	size_t i, j;
	int8_t c;

	for (i = 0, j = strlen(s1) - 1; i < j; i++, j--) {
		c = s1[j];
		s1[j] = s1[i];
		s1[i] = c;
	}
}

