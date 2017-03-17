/*
 * Exercise 1-11. How would you test the word count program? What kinds of
 * input are most likely to uncover bugs if there are any?
 */

#include <stdio.h>

#define IN	1	/* inside of a word */
#define OUT	0	/* outside of a word */

/* count lines words and characters in input */

int main(void)
{
	int i, c, nl, nw, nc, state;

	state = OUT;
	nl = nw = nc = 0;

	while ((c = getchar()) != 'Q')
	{
		++nc;
		if (c == '\n')
			++nl;
		if (c == ' ' || c == '\n' || c == '\t' || c == '\v')
			state = OUT;
		else if (state == OUT)
		{
			state = IN;
			++nw;
		}
	}
	printf("nl = %d nw = %d nc = %d\n", nl, nw, nc);

	return 0;
}
