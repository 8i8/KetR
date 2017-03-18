/*
 * Exercise 1-13. Write a program to print a histogram of the lengths of words
 * in its input. It is easy to draw the histogram with the bars horizontal; a
 * vertical orientation is more challenging.
 */
#include <stdio.h>

#define IN 	1
#define OUT 	0
#define MAXLEN	1000

int main (void)
{
	int string[MAXLEN];
	int i, j, k, c, wc, wl, lw, state;

	state = OUT;
	wc = wl = lw = 0;

	/*
	 * Count the words in the input string and track the char in
	 * the longest word.
	 */
	i = 0;
	while ((c = getchar()) != EOF && i < MAXLEN-1)
	{
		string[i++] = c;
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
	string[i++] = '\0';

	/*
	 * Print the findings and reset the counters.
	 */
	char array[wc][lw];
	printf("Num Words : %d, Longest word : %d\n\n", wc, lw);
	state = OUT;
	wl = wc = 0;

	/*
	 * Fill array with the histograms graphic characters.
	 */
	for  (i = 0; string[i] != '\0'; i++)
	{
		c = string[i];

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
		for (k = 0; k < wc; k++)
			printf("%c", array[k][j]);
		printf("\n");
	}
}

