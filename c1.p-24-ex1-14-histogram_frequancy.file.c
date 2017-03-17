/*
 * Exercise 1-14. Write a program to print a histogram of the frequencies of
 * different characters in its input.
 */
#include <stdio.h>

#define CHAR_OFFSET	32
#define RANGE		95

int main(int argc, char *argv[])
{
	int i, j, k, c;
	int max;

	char letters[RANGE][2];

	i = max = 0;

	/*
	 * Fill array with the printable characters from the standard ASCI char
	 * set, the corresponding second array column to 0.
	 */
	while (i < RANGE)
	{
		letters[i][0] = i + CHAR_OFFSET;
		letters[i][1] = 0;
		i++;
	}

	for (k = 1; k < argc; k++) {

		FILE *file = fopen(argv[k], "r");

		/*
		 * Check that a is file availble.
		 */
		if (file == NULL)
			return 1;

		/*
		 * Count the occurrences of each character adding to the value
		 * in the second array table each time, obtain the length of
		 * the array required to display the histogram.
		 */
		while ((c = fgetc(file)) != EOF)
		{
			for (i = 0; i < RANGE; i++)
				if (letters[i][0] == c) {
					letters[i][1]++;
					/* Track the highest character count */
					if (letters[i][1] > max)
						max = letters[i][1];
				}
		}

		/*
		 * Fill the output array with the glyphs required to draw the
		 * histogram, notice that the loop also writes a blank space,
		 * if the pipe is not required, this is to insure that any
		 * parasites are removed from the array, should it be the case
		 * that the screen is being refreshed.
		 */
		char output[RANGE][max];
		/* */
		for (j = 0; j < max; j++)
			for (i = 0; i < RANGE; i++)
			{
				if (j < letters[i][1])
					output[i][j] = '|';
				else
					output[i][j] = ' ';
			}

		/*
		 * Print the histogram to screen, notice the inversion of the i
		 * and j for the output, also that j is counted down from the
		 * maximum length until 0 as the terminal displays from the top
		 * down.
		 */
		for (j = max-1; j >= 0; j--) {
			for (i = 0; i < RANGE; i++)
				printf("%c", output[i][j]);
			putchar('\n');
		}

		for (i = 0; i < RANGE; i++) {
			printf("%c", letters[i][0]);
		}
		putchar('\n');

		fclose(file);
	}
	return 0;
}
