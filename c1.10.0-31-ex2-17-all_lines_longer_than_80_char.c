/*
 * Exercise 1-17. Write a program to print all input lines that are longer than
 * 80 characters.
 */
#include <stdio.h>

#define MAXLENGTH	80

void findLongestLine(FILE *file, int data[]);
void printLinesOver80(FILE *file, int maxlen);

int main(int argc, char *argv[])
{
	int i;
	int data[] = {0,0};

	for (i = 1; i < argc; i++)
	{
		FILE *file = fopen(argv[i], "r");

		if (file == NULL)
			return 1; /*Could not open file*/

		findLongestLine(file, data);
		printf("There are  : %d lines over 80 characters in length\n"
				, data[0]);
		printf("longest line : %d characters\n", data[1]);
		rewind(file);

		printLinesOver80(file, data[1]);
		fclose(file);
	}
}

void findLongestLine(FILE *file, int data[])
{
	int c, len, maxlen, count;
	len = maxlen = count = 0;

	while ((c = fgetc(file)) != EOF)
	{
		if (c == '\n') {
			if (len > MAXLENGTH)
				count++;
			if (len > maxlen)
				maxlen = len;
			len = 0;
		}
		len++;
	}
	data[0] = count;
	data[1] = maxlen;
}

void printLinesOver80(FILE *file, int maxlen)
{
	int c, len;
	char lines[maxlen];
	len = 0;

	while ((c = fgetc(file)) != EOF)
	{
		if (c != '\n') {

			lines[len] = c;
			len++;
		}
		if (c == '\n' && len > MAXLENGTH) {

			lines[len+1] = '\0';
			printf("%s\n\n", lines);
			len = 0;
		}
		if (c == '\n')
			len = 0;
	}
}
