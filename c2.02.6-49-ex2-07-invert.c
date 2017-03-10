/*
 * Exercise 2-7. Write a function invert(x,p,n) that returns x with the n bits
 * that begin at position p inverted (i.e., 1 changed into 0 and vice versa),
 * leaving the others unchanged.
 */
#include <stdio.h>

int invert(int x, int p, int n)
{
	int mask;
	int sect;

	mask = ~(~0 << n) << (p+1-n);

	sect = mask & x;
	sect = ~sect >> (p+1-n) & ~(~0 << n);
	sect = sect << (p+1-n);

	x = x & ~mask;
	x = x | sect;

	return x;
}

int main(void)
{
	int x = 12345;
	int p = 9;
	int n = 5;
	int y;

	y = invert(x, p, n);
	printf("getbits(%u %x), %d, %d) = %u (%x)\n", x, x, p, n, y, y);
	if(y == 13273)
		puts("Woohooo");
}

