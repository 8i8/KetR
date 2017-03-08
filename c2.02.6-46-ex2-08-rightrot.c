/*
 * Exercise 2-8. Write a function rightrot(x,n) that returns the value of the
 * integer x rotated to the right by n positions.
 */
#include <stdio.h>

int rightrot(unsigned int x, int n);

int main(void)
{
	unsigned int c, x, n;

	puts("Press any key to start.");

	while ((c = getchar()) != 'Q')
	{
		puts("Enter an integer to rotate right: ");
		scanf("%u", &x);
		puts("How many places? ");
		scanf("%u", &n);

		x = rightrot(x, n);
		printf("--> %u\n", x);
	}

	return 0;
}

int rightrot(unsigned int x, int n)
{
	return x << n;
}

