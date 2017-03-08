/*
 * Exercise 2-6. Write a function setbits(x,p,n,y) that returns x with the n
 * bits that begin at position p set to the rightmost n bits of y, leaving the
 * other bits unchanged.
 *
 * 010yyyy1xxxx0101
 *
 * x1 = x >> (p+1-n) & ~(~0 << n)
 *
 * 000000000000xxxx
 *
 * x1 << y+1-n
 *
 * 000xxxx000000000
 *
 * ============================
 *
 * 010yyyy1xxxx0101
 *
 * x2 = x & ~((~(~0 << n)) << y+1-n)
 *
 * 1110000111111111
 *
 * ==
 *
 * 01000001xxxx0101
 *
 * x2 | x1
 *
 * 000xxxx000000000
 *
 * ============================
 *
 * 010xxxx1xxxx0101
 *
 */
#include <stdio.h>

int setbits(int x, int p, int n, int y);

int main(void)
{
	int x = 12345;
	int p = 7;
	int n = 4;
	int y = 12;
	int z;

	z = setbits(x, p, n, y);
	printf("getbits(%u %x), %d, %d) = %u (%x)\n", x, x, p, n, z, z);
	if(z == 9785)
		puts("Woohooo");
}

/*
 * Setbits 'n' from position 'x' to position 'y'.
 */
int setbits(int x, int p, int n, int y)
{
	int x1;
	int x2;

	x1 = (x >> (p+1-n) & ~(~0 << n)) << (y+1-n);
	x2 = x & ~((~(~0 << n)) << (y+1-n));

	return x2 | x1;
}

