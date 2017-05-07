/*
 * Exercise 5-10. Write the program expr, which evaluates a reverse Polish
 * expression from the command line, where each operator or operand is a
 * separate argument. For example, expr 2 3 4 + * evaluates 2 * (3+4).
 */
#include <stdio.h>
#include <ctype.h>

void read_arg(char* argv)
{
	char c;

	while(getop(*argv))
	{
		c = *argv++;

		if (isdigit(c))
			;
		if (isalpha(c))
			;
	}
}

int main(int argc, char *argv[])
{
	while (--argc > 0) {
		read_arg(*++argv);
	}

	return 0;
}

