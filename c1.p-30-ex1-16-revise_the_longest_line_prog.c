/*
 * Exercise 1-16. Revise the main routine of the longest-line program so it
 * will correctly print the length of arbitrary long input lines, and as much
 * as possible of the text.
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

#define MAXLEN	1000	/* Maximum length of chatracter input */

static size_t getline(char line[], size_t maxline);
static void copy(char to[], char from[]);

int main(void)
{
	size_t len;		/* Current line length */
	size_t max;		/* Maximum length seen so far */

	char line[MAXLEN];
	char longest[MAXLEN];

	max = 0;
	while ((len = getline(line, MAXLEN)) > 0)
		if (len > max) {
			max = len;
			copy(longest, line);
		}

	if (max > 0) {		/* There was a line */
		printf("The longest line is %lu characters long, and here it is:\n", max);
		printf("%s\n", longest);
	}

	return 0;
}

/*
 * Read input line by line.
 */
static size_t getline(char str[], size_t lim)
{
	size_t i;
	int c;

	for (i = 0; i < lim-1 && (c = getchar()) != EOF && c != '\n'; i++)
		str[i] = c;

	if (c == '\n')
		str[i++] = c;
	str[i] = '\0';
	return i;
}

/*
 * Copy 'from' into 'to', assuming that 'to' is large enough.
 */
static void copy(char to[], char from[])
{
	int i;

	i = 0;
	while ((to[i] = from[i]) != '\0')
		i++;
}

