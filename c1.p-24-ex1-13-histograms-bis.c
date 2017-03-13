/*
 * Exercise 1-13. Write a program to print a histogram of the lengths of words
 * in its input. It is easy to draw the histogram with the bars horizontal; a
 * vertical orientation is more challenging.
 */
#include <stdio.h>

#define IN 	1
#define OUT 	0

int main (int argc, char *argv[])
{
	int i, j, k, c, wc, wl, lw, state;

	state = OUT;
	wc = wl = lw = 0;

	for (i = 1; i < argc; i++) {

		FILE *file = fopen(argv[i], "r");

		if (file == NULL)
			return 1; //Can not open file

		while ((c = fgetc(file)) != EOF)
		{
			if (c == ' ' || c == '\n' || c == '\t') {
				state = OUT;
				wl = 0;
			} else if (state == OUT) {
				state = IN;
				wc++;
			}

			if (state == IN)
				wl++;
			
			if (wl > lw)
				lw = wl;
		}

		char array[wc][lw];

		rewind(file);

		printf("Num Words : %d, Longest word : %d\n\n", wc, lw);

		state = OUT;
		wl = wc = 0;

		while ((c = fgetc(file)) != EOF)
		{
			if (c == ' ' || c == '\n' || c == '\t') {

				while (state == IN && wl < lw)
				{
					array[wc-1][wl] = ' ';
					wl++;
				}

				state = OUT;
				wl = 0;

			} else if (state == OUT) {
				state = IN;
				wc++;
			}

			if (state == IN) {
				array[wc-1][wl] = '|';
				wl++;
			}
		}
		for (j = lw-1; j >= 0; j--) {
			k = 0;
			while (k < wc)
			{
				printf("%c", array[k][j]);
				k++;
			}
			printf("\n");
		}
		fclose(file);
	}
}
