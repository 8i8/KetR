/*
 * `Exercise 3-3. Write a function expand(s1,s2) that expands shorthand
 * notations like a-z in the string s1 into the equivalent complete list
 * abc...xyz in s2. Allow for letters of either case and digits, and be
 * prepared to handle cases like a-b-c and a-z0-9 and -a-z. Arrange that a
 * leading or trailing - is taken literally.
 *
 * TODO Check on the optimisation of the function, see comment at line 123.
 */

/* Redefine getline */
#undef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <stdint.h>
#include <ctype.h>

#define MAXLEN		255

static size_t __expandRead(const char read[], char s2[], size_t j);
static size_t __checkState(const char read[], char s2[], const size_t j);
static void __writeInputBuffer(char read[], const int8_t c);
static void expand(const char s1[], char s2[]);
static size_t getline(char str[], const size_t lim);

int main(void)
{
	char s1[MAXLEN];
	char s2[MAXLEN];

	while ((getline(s1, MAXLEN)) > 0)
	{
		expand(s1, s2);
		printf("%s", s2);
	}

	return 0;
}

/*
 * Expand the expression, either ascending of descending, return the number of
 * char that have been added.
 */
static size_t __expandRead(const char read[], char s2[], size_t j)
{
	size_t i;
	i = 1;		/* The count starts at 1 for a-z [0] is already an a. */
	j--;		/* Diminish j to account for the hyphen. */

	if (read[0] < read[2])
		while (read[0] + i <= read[2])
			s2[j++] = read[0] + i++;
	else if (read[0] > read[2])
		while (read[0] - i >= read[2])
			s2[j++] = read[0] - i++;
	return j;
}

/*
 * Check the char both before and after the hyphen, are they the same type? If
 * yes then expand the expression.
 */
static size_t __checkState(const char read[], char s2[], const size_t j)
{
	if (!isalnum(read[0]) || !isalnum(read[2]))
		;/* Returns 0 lone hypen will not be removed -a-z */
	else if (islower(read[0]) && islower(read[2]))
		return __expandRead(read, s2, j);
	else if (isupper(read[0]) && isupper(read[2]))
		return __expandRead(read, s2, j);
	else if (isdigit(read[0]) && isdigit(read[2]))
		return __expandRead(read, s2, j);
	return 0;
}

/*
 * Keep store of the last three characters entered, used to define the state
 * for expansion, if it is required.
 */
static void __writeInputBuffer(char read[], const int8_t c)
{
	read[0] = read[1];
	read[1] = read[2];
	read[2] = c;
}

/*
 * Receive two strings assuming that they are of adequate length for the
 * operation, iterate over string 1 transferring to string t, use a buffer of
 * 3 char so that any 3 char expressions such as a-z can be expanded into the
 * second string.
 */
static void expand(const char s1[], char s2[])
{
	size_t i, j;
	char read[3];
	uint8_t buffer, count, num;
	count = i = j = 0;

/*
 * Create a buffer for reading ahead 2 char, enables the expansion of 3 char
 * expressions such as a-z, even on the fly (whilst streaming).
 */
	buffer = 3;
	while(buffer && s1[i] != '\0')
	{
		__writeInputBuffer(read, s1[i]);
		buffer--, i++;
	}

	i = 0;
	while(s1[i] != '\0')
	{
		__writeInputBuffer(read, s1[i]);

		/* If an expansion has just been made, skip over the hyphen */
		if(count)
			count--;
		else {
			if(read[1] == '-') {
/*
 * I am not certain that this code is optimal: Here j is diminished to account
 * for the hyphen in the expression, the value of j returned by checkState
 * includes the last itteration of expandRead after the <= or >=; It should be
 * the case that this char can be used directly from s1 and thus one less
 * itteration be required.
 *
 * If __checkState returns a value of j, then the expression has been expanded
 * and the count is set to 1. Such that the loop skip over the hyphen.
 *
 * In effect: Should count be set to 2, and the equal sign removed from both
 * the <= and the >= expressions in expandRead.
 */
				if((num = __checkState(read, s2, j)) > 0) {
					count = 1;
					j = num;
					i++;
				} else
					s2[j++] = s1[i++];
			} else
				s2[j++] = s1[i++];
		}
	}
	s2[j] = '\0';
}

/*
 * Get input line by line.
 */
static size_t getline(char str[], const size_t lim)
{
	size_t i;
	int8_t c;

	for (i = 0; i < lim-1 && (c = getchar()) != EOF && c != '\n'; i++)
		str[i] = c;

	if(c == '\n')
		str[i++] = '\n';

	str[i] = '\0';

	return i;
}

