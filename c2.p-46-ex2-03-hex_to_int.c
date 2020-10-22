/*
 * Exercise 2-3. Write a function htoi(s), which converts a string of
 * hexadecimal digits (including an optional 0x or 0X) into its equivalent
 * integer value. The allowable digits are 0 through 9, a through f, and A
 * through F.
 */

#undef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <ctype.h>

#define MAXLEN	20

int htoi(char string[], int len);
int myPow(int base, int exp);
int ipow(int base, int exp);
int getHexValue(char c);
int lower(int c);
int getline(char string[], int lim);

int main(void)
{
	char string[MAXLEN];
	int len;
	int result;

	printf("Please enter a hexadecimal value to convert into decimal:\n");

	while ((len = getline(string, MAXLEN)) > 0)
	{
		result = htoi(string, len);
		printf("--> %d\n", result);
	}
}

/*
 * My hexadecimal to decimal function.
 */
int htoi(char string[], int len)
{
	int i;
	int j;
	int int_c;
	char c;
	int x;

	i = x = j = int_c = 0;

	for (i = len-1; i >= 0; i--)
	{
		c = string[i];
		int_c = c - '0';
		if (isdigit(string[i]) && j > 0)
			x = x + ( myPow(16, j) * int_c );
		else if (isdigit(string[i]))
			x = x + int_c;
		else if (j > 0)
			x = x + ( myPow(16, j) * getHexValue(string[i]));
		else 
			x = x + getHexValue(string[i]);
		++j;
	}
	return x;
}

/*
 * My power function, as the program is only concerned with integers the
 * <math.h> function would be overkill, so here is a local version that does
 * not require doubles.
 */
int myPow(int base, int exp)
{
	int i;
	int result;

	result = base;

	for(i = 0; i < exp-1; i++)
		result = result * base;

	return result;
}

/*
 * I will be broaching bitwise operation in the next section, until then here
 * is another solution to the above function that is much more succinct. I will
 * aspire to its full understanding.
 *
 * Note, the right shift in this function is implimentation defined and as such
 * this code is not reasonable.
 */
int ipow(int base, int exp)
{
	int result = 1;

	while (exp)
	{
		if (exp & 1)
			result *= base;
		exp >>= 1;
		base *= base;
	}
	return result;
}

/*
 * return the base 10 equivalent of the hexadecimal letters.
 */
int getHexValue(char c)
{
	c = lower(c);

	int x;

	if	(c == 'a')
		x = 10;
	else if (c == 'b')
		x = 11;
	else if (c == 'c')
		x = 12;
	else if (c == 'd')
		x = 13;
	else if (c == 'e')
		x = 14;
	else if (c == 'f')
		x = 15;

	return x;
}

/*
 * Convert to lower case.
 */
int lower(int c)
{
	if (c >= 'A' && c <= 'F')
		return c + 'a' - 'A';
	else
		return c;
}

/*
 * Retreive user input.
 */
int getline(char string[], int lim)
{
	int i;
	int c;

	for (i = 0; i < lim-1 && (c = getchar()) != EOF && c != '\n'; i++)
	{
		if(isxdigit(c))
			string[i] = c;
		else
			printf("Please use only hexadecimal characters.\n");
	}

	return i;
}

