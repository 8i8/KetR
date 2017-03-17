/*
 * `Exercise 3-3. Write a function expand(s1,s2) that expands shorthand
 * notations like a-z in the string s1 into the equivalent complete list
 * abc...xyz in s2. Allow for letters of either case and digits, and be
 * prepared to handle cases like a-b-c and a-z0-9 and -a-z. Arrange that a
 * leading or trailing - is taken literally.
 */
#include <stdio.h>
#define LIMIT	1000

int main(void)
{
	char input[LIMIT];
	int len;

	len = getInput(input, LIMIT);
	char s[len];
	defineString(input, s);
	pString("initial string", s);

	return 0;
}

/*
 * Print string to terminal.
 */
void pString(char pre[], char string[])
{
	printf("%s\t-->\t%s\n", pre, string);
}

/*
 * Copy the input string into a string of the correct size.
 */
void defineString(char input[], char s[])
{
	int i;
	i = 0;
	while (input[i]) {
		s[i] = input[i];
		i++;
	}
	s[i++] = '\0';
}

/*
 * Get a text input from the user.
 */
int getInput(char input[], int lim)
{
	int i;
	int c;

	for(i = 0; i < lim-2 && ((c = getchar()) != '\n'); i++)
		input[i] = c;

	if (c == '\n')
		input[i++] = '\n';

	input[i++] = '\0';
	return i;
}

