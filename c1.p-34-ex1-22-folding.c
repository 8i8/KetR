/*
 * Exercise 1-22. Write a program to ``fold'' long input lines into two or more
 * shorter lines after the last non-blank character that occurs before the n-th
 * column of input. Make sure your program does something intelligent with very
 * long lines, and if there are no blanks or tabs before the specified column.
 */
/*
 * The function getline() will not compile under the modern GCC compilers
 * default configuration due to its inclusion as a function in the <stdio.h>
 * header file, this can be resolved in three ways, either; by using the -ansi
 * GCC compiler flag; As explained in the GNU libc documentation.
 *
 * https://www.gnu.org/software/libc/manual/html_node/Using-the-Library.html#Using-the-Library
 * https://www.gnu.org/software/libc/manual/html_node/Feature-Test-Macros.html
 *
 * Else, the following definition change can be made, so as to allow the use of
 * getline() in the example without having to call it something else, it is
 * required that the definition be made before including stdio.h.
 *
 * https://github.com/ptdecker/cbasics/blob/master/src/chapter01/longestline.c
 *
 * TODO This code is unfinished the tab folding needs to be worked on, I think
 * that this could be an interesting problem to write a unit test for.
 */

#undef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <stdint.h>
#include <ctype.h>

#define BUFFER		255
#define FOLD		80
#define TABWIDTH	8
#define NORMAL		0
#define HYPHEN		1

/*
 * Does what it says, however also worthy of note: Has a fancy parameter
 * 'ending' which specifies how the end of the line is treated in regards to
 * hyphen breaks and carriage returns.
 */
static void printLine(char line[], uint8_t head, uint8_t ending)
{
	uint8_t i;

	for (i = 0; i < head; i++)
		printf("%c", line[i]);
	if (ending) {
		printf("-\n");
	} else
		putchar('\n');
}

/*
 * Add to tw, the virtual end of the line, used to offset the value of head
 * which is itself the point on the actual array.
 */
static uint8_t offsetTab(char line[], uint8_t head)
{
	uint8_t tw;

	tw = head + TABWIDTH;
	if (tw >= FOLD) {
		printLine(line, head, 0);
		return 0;
	}
	return tw;
}

/*
 * When the line is printed and the space place marker is used, it is necessary
 * to copy the remaining characters that have been read into the array, into
 * the arrays beginning, for that next line.
 */
static uint8_t copyEnding(char line[], uint8_t head, uint8_t marker)
{
	uint8_t i;

	for (i = marker; i < head; i++)
		line[i - marker] = line[i];

	return head - marker;
}

/*
 * Create a 2 char buffer to allow the line wrap to be awear if the next char
 * is a space; This is to stop folds occuring should the line end upon the end
 * of a word.
 */
static int inputBuffer(char input[])
{
	int c;

	c = getchar();
	input[0] = input[1];
	input[1] = c;

	return input[0];
}

/*
 * It all starts here.
 */
int main(void)
{
	char line[BUFFER];
	char input[2];
	uint8_t marker;
	uint8_t head;
	uint8_t tw;

	head = tw = 0;

	while (inputBuffer(input) != EOF)
	{
		line[head] = input[0];

		if (tw >= FOLD) {
			if (isspace(input[1])) {
				printLine(line, ++head, NORMAL);
				head = marker = tw = 0;
				continue;
			} else if(marker) {
				printLine(line, marker, NORMAL);
				tw = head = copyEnding(line, ++head, marker);
				marker = 0;
				continue;
			} else {
				printLine(line, --head, HYPHEN);
				head = copyEnding(line, head, marker);
				tw -= marker;
				marker = 0;
				continue;
			}
		} else if (line[head] == '\n') {
			printLine(line, head, NORMAL);
			head = marker = tw = 0;
			continue;
		} else if (line[head] == '\t') {
			marker = head+1;
			tw = offsetTab(line, head);
		} else if (line[head] == ' ') {
			marker = head+1;
		}
		head++, tw++;
	}
	return 0;
}

