/*
 * Exercise 1-18. Write a program to remove trailing blanks and tabs from each
 * line of input, and to delete entirely blank lines.
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
#include <ctype.h>

#define MAXLEN	1000

static size_t clearEnd(char line[], size_t len);
static size_t getline(char line[], size_t len);

int main(void)
{
	size_t len;
	char line[MAXLEN];

	while (!feof(stdin))
	{
		len = getline(line, MAXLEN);
		if (len) {
			len = clearEnd(line, len);
			printf("%s", line);
		}
	}
}

/*
 * Remove trailing spaces and tabs.
 */
static size_t clearEnd(char line[], size_t len)
{
	while (--len > 0 && (!isgraph(line[len])) )
		;
	if(line[++len] == '\n')
		++len;
	line[len] = '\0';

	return len;
}

/*
 * Read a line in to an array from a text file, return the arrays used length.
 */
static size_t getline(char line[], size_t lim)
{
	int c;
	size_t len, i;

	i = len = 0;
	while (--lim > 0 && (c = getchar()) != EOF && c != '\n') {
		line[i++] = c;
		len++;
	}
	if (c == '\n')
		line[i++] = c;
	line[i] = '\0';

	return len;
}

