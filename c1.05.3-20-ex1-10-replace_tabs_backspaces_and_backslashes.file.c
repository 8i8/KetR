/*
 * Ex 1-10 Write a program that copys its input to its output, replacing each
 * tab by \t each backspace by \b and each backslash by \\.
 */
#include <stdio.h>

int main(int argc, char *argv[])
{
	int i, c;

	for (i = 1; i < argc; i++) {

		FILE *file = fopen(argv[i], "r");

		if (file == NULL)
			return 1; //Could not open file

		while ((c = fgetc(file)) != EOF)
		{
			if (c == '\t')
				printf("\\t");
			else if (c == '\b')
				printf("\\b");
			else if (c == '\\')
				printf("\\");
			else
				printf("%c", c);
		}
		fclose(file);
	}
}
