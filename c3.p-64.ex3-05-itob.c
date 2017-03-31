/*
 * Exercise 3-5. Write the function itob(n,s,b) that converts the integer n
 * into a base b character representation in the string s. In particular,
 * itob(n,s,16) formats s as a hexadecimal integer in s.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <stdint.h>

#define MAXLEN		65536

void itob(uint32_t n, char s[], const uint8_t base);
static char __cnvBase(const int8_t n, const uint8_t base);
static char __itoh(const int8_t n);
static char __itoo(const int8_t n);
static void __reverse(char s1[]);

int main(void)
{
	char s[MAXLEN];
	uint32_t n;

	while (!feof(stdin))
	{
		scanf("%u", &n);
		printf("The value of n as an hex       :  %x\n", n);
		itob(n, s, 16);
		printf("The value of n as a hex string : \"%s\"\n", s);
		printf("The value of n as an oct       :  %o\n", n);
		itob(n, s, 8);
		printf("The value of n as a oct string : \"%s\"\n", s);
	}

	return 0;
}

/*
 * Transform a value of the int type into a string.
 */ 
void itob(uint32_t n, char s[], const uint8_t base)
{
	size_t i;
	uint32_t sign;

	/* record the sign */
	sign = n;

	i = 0;
	do {
		/* make n positive and generate digits in reverse order */
		s[i++] = __cnvBase(abs(n % base), base);
	}
	while ((n /= base) != 0);

	/*
	 * Add the sign if it is required, redundant so long as sthe function
	 * takes unsigned int.
	 */
	if (sign < 0)
		;

	s[i] = '\0';

	__reverse(s);
}

/*
 * Convert to which base?
 */
static char __cnvBase(const int8_t n, const uint8_t base)
{
	if (base == 16)
		return __itoh(n);
	else if (base == 8)
		return __itoo(n);
	return 0;
}

/*
 * Convert an int between 0 and 16 into hex equivalent.
 */
static char __itoh(const int8_t n)
{
	if(n > -1 && n < 10)
		return (char)n + '0';
	else if (n > 9 && n < 17)
		return (char)n + 87;
	return 0;
}

/*
 * Convert an int between 0 and 7 into octal equivalent.
 */
static char __itoo(const int8_t n)
{
	return (char)n + '0';
}

/*
 * Reverse the array.
 */
static void __reverse(char s1[])
{
	size_t i, j;
	char c;

	for (i = 0, j = strlen(s1) - 1; i < j; i++, j--) {
		c = s1[j];
		s1[j] = s1[i];
		s1[i] = c;
	}
}

