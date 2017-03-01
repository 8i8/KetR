/*
 * Exercise 1-14. Write a program to print a histogram of the frequencies of
 * different characters in its input.
 */
#include <stdio.h>

int main(int argc, char *argv[])
{
	int i, j, c;

	char letters[127][2];

	i = 0;

	while (i < 127)
	{
		letters[i][0] = i;
		letters[i][1] = 0;
		i++;
	}

	for (i = 1; i < argc; i++) {

		FILE *file = fopen(argv[i], "r");

		if (file == NULL)
			return 1; //Can not opend file.

		while ((c = fgetc(file)) != EOF)
		{
			for (j = 0; j < 127; j++)
				if (j == c)
					letters[j][1]++;
		}

		for (j = 0; j < 127; j++) {
			printf("%c → %d\n", letters[j][0], letters[j][1]);
		}
		fclose(file);
	}
}
