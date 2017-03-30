/*
 * `Exercise 3-3. Write a function expand(s1,s2) that expands shorthand
 * notations like a-z in the string s1 into the equivalent complete list
 * abc...xyz in s2. Allow for letters of either case and digits, and be
 * prepared to handle cases like a-b-c and a-z0-9 and -a-z. Arrange that a
 * leading or trailing - is taken literally.
 */

#undef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <stdint.h>
#include <ctype.h>

/*
 * Expand the expression, either asscending of decending.
 */
static void expandRead(const char read[])
{
	int8_t i;
	i = 0;

	if (read[0] < read[2])
		while (read[0]+i < read[2])
			printf("%c", read[0] + i++);
	else if (read[0] > read[2])
		while (read[0] - i > read[2])
			printf("%c", read[0] - i++);
}

/*
 * Check the char both before and after the hyphen, are they the same type? If
 * yes then expand the expression.
 */
static uint8_t checkState(const char read[])
{
	if (!isalnum(read[0]))
		;
	else if (islower(read[0]) && islower(read[2])) {
		expandRead(read);
		return 1;
	}
	else if (isupper(read[0]) && isupper(read[2])) {
		expandRead(read);
		return 1;
	}
	else if (isdigit(read[0]) && isdigit(read[2])) {
		expandRead(read);
		return 1;
	}
	return 0;
}

/*
 * Keep store of the last three characters entered, used to define the state
 * for expansion if required.
 */
static void writeInputBuffer(char read[], const int8_t c)
{
	read[0] = read[1];
	read[1] = read[2];
	read[2] = c;
}

int main(void)
{
	int8_t  c;
	uint8_t count;
	char read[3];

	/*
	 * Create the offset for reading ahead 2 char, enables the expansion of
	 * 3 char expressions such as a-c
	 */
	count = 2;
	while ((c = getchar()) != EOF)
	{
		writeInputBuffer(read, c);

		/* If expansion has just been made, skip over the hyphen */
		if(count)
			count--;
		else {
			if(read[1] == '-') {
				if(checkState(read))
					count = 1;
				else
					putchar(read[0]);
			} else
				putchar(read[0]);
		}
	}
	putchar('\n');

	return 0;
}

