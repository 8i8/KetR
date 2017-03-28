/*
 * The C programming language, second edition.
 *
 * Exercise 1-22. Write a program to ``fold'' long input lines into two or more
 * shorter lines after the last non-blank character that occurs before the n-th
 * column of input. Make sure your program does something intelligent with very
 * long lines, and if there are no blanks or tabs before the specified column.
 */
#include <stdio.h>
#include <stdint.h>
#include <ctype.h>

#define DEBUG		2
#define BUFFER		255
#define FOLD		40
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
 * which is itself the reading head for the array.
 */
static uint8_t offsetTab(char line[], uint8_t tw, uint8_t head)
{
	tw = (tw / TABWIDTH+1) * TABWIDTH;

	if (tw >= FOLD) {
		printLine(line, head, NORMAL);
		return 0;
	}
	return tw;
}

/*
 * When the line is printed and the space 'place marker' is used, it is
 * necessary to copy the remaining characters that have been read into the
 * array, into the arrays beginning, for that next line.
 */
static uint8_t copyEnding(char line[], uint8_t head, uint8_t marker)
{
	uint8_t i;

	for (i = ++marker; i < head; i++)
		line[i - marker] = line[i];

	return head - marker;
}

/*
 * It all starts here, well, sort of.
 */
int main(void)
{
	char line[BUFFER];

	uint8_t marker;
	uint8_t head;
	uint8_t isSkipOnce;
	uint8_t tw;

	head = tw = isSkipOnce = 0;

	while ((line[head] = getchar()) != EOF)
	{
		if (tw >= FOLD) {
			if (isspace(line[head])) {
				printLine(line, head, NORMAL);
				head = marker = tw = 0;
				continue;
			} else if (isspace(line[FOLD+1])) {
				printLine(line, head, NORMAL);
				head = marker = tw = 0;
				continue;
			} else if (marker) {
				printLine(line, marker, NORMAL);
				head = copyEnding(line, ++head, marker);
				tw = head;
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
			marker = head;
			tw = offsetTab(line, tw, head);
			if(!tw) {
				head = marker = 0;
				continue;
			}
		} else if (line[head] == ' ') {
			marker = head;
		}
		head++, tw++;
	}
	return 0;
}

