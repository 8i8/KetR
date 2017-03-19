/*
 * Exercise 1-17. Write a program to print all input lines that are longer than
 * 80 characters.
 */

/*
 * The function getline() will not compile under the moddern GCC compilers
 * defalut configuration due to its inculsion as a function in the <stdio.h>
 * header file, this can be resolved in three ways, either; by using the -ansi
 * GCC compiler flag; As explaind in the GNU libc documentation.
 *
 * https://www.gnu.org/software/libc/manual/html_node/Using-the-Library.html#Using-the-Library
 * https://www.gnu.org/software/libc/manual/html_node/Feature-Test-Macros.html
 *
 * Else, the following definition change can be made, so as to allow the use of
 * getline() in the example without having to call it something else, it is
 * required that the definition be made before including stdio.h.
 *
 * https://github.com/ptdecker/cbasics/blob/master/src/chapter01/longestline.c
 */

#undef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200112L

#include <stdio.h>

#define MAXLEN		1000
#define TARGETLEN	80

static size_t getline(char line[], size_t lim);

int main(void)
{
	size_t len;
	char line[MAXLEN];

	while (!feof(stdin)) {
		len = getline(line, MAXLEN);
		if (len > TARGETLEN)
			printf("%lu:%s", len, line);
	}
	return 0;
}

static size_t getline(char line[], size_t lim)
{
	int c;
	size_t i;
	size_t len;

	len = i = 0;
	while (--lim > 0 && (c = getchar()) != EOF && c != '\n') {
		line[i++] = c;
		len++;
	}
	if(c == '\n')
		line[i++] = c;
	line[i] = '\0';

	return len;
}

