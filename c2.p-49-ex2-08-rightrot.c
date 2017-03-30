/*
 * Exercise 2-8. Write a function rightrot(x,n) that returns the value of the
 * integer x rotated to the right by n positions.
 */
#include <stdio.h>
#include <stdint.h>

static uint64_t rightrot(uint64_t x, uint64_t n);

int main(void)
{
	uint64_t c, x, n;

	puts("Enter an integer to rotate right: ");
	scanf("%lu", &x);

	while ((c = getchar()) != EOF)
	{
		puts("How many places? ");
		scanf("%lu", &n);

		x = rightrot(x, n);
		printf("--> %lu\n", x);
		n = x;
	}

	return 0;
}

static uint64_t rightrot(uint64_t x, uint64_t n)
{
	return x << n;
}

