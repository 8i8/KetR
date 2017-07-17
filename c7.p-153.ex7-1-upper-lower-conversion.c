/*
 *  Exercise 7-1. Write a program that converts upper case to lower or lower
 *  case to upper, depending on the name it is invoked with, as found in
 *  argv[0].
 */
#include <stdio.h>
#include <ctype.h>

static int error(void)
{
	printf("Usage: prog -l  conver text to lower case.\n");
	printf("       prog -u  conver text to upper case.\n");
	return 1;
}

int main(int argc, char* argv[])
{
	char c;
	unsigned short mode;

	if (argc == 1 || (*argv)[0] != '-')
		return error();
			
	while (--argc > 0 && (*++argv)[0] == '-')
		while ((c = *++argv[0]))
			switch (c) {
				case 'l':
					mode = 0;
					break;
				case 'u':
					mode = 1;
					break;
				default:
					return error();
			}

	while ((c = getchar()) != EOF) {
		if (isalpha(c)) {
			if (mode)
				c = toupper(c);
			else
				c = tolower(c);
		}
		putchar(c);
	}

	return 0;
}

