/*
 * Exercise 3-4. In a two's complement number representation, our version of
 * itoa does not handle the largest negative number, that is, the value of n
 * equal to -(2 wordsize-1 ).  Explain why not. Modify it to print that value
 * correctly, regardless of the machine on which it runs.
 *
 * The unary operator - will not work for INT_MIN this is because the number
 * does not exist, for the int type the least negative value is greater than
 * the highest posative.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <stdint.h>

#define MAXLEN	1000

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

/*
 * Transform a value of the int type into a string.
 */ 
void itoa(int32_t n, char s[])
{
	size_t i;
	int8_t sign;

	/* record the sign */
	sign = n;

	/*
	 * By using abs() from stdlib.h which performs the same unary
	 * operation, we can make the change on every iteration of the do while
	 * loop, by doing this the highest negative value is never parsed, thus
	 * solving the problem by never performing -n on the lowest negative
	 * integer.
	 */
	i = 0;
	do {
		/* make n positive and generate digits in reverse order */
		s[i++] = (char)abs(n % 10) + '0';
	}
	while ((n /= 10) != 0);

	/* Add the sign if it is required */
	if (sign < 0)
		s[i++] = '-';

	s[i] = '\0';

	__reverse(s);
}

int main(void)
{
	char s[MAXLEN];
	int32_t n;
	int32_t o;
        n = INT32_MIN;
        o = INT32_MIN;
	o = -o;

	printf("The value of INT_MIN as    : %d\n", n);
	printf("The value of n after -n    : %d\n", o);
	itoa(n, s);
	printf("The value of n as a string : \"%s\"\n", s);

	return 0;
}

