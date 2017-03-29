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
#include <stdint.h>

static uint16_t setbits(uint16_t x, uint8_t p, uint8_t n, uint8_t y);

int main(void)
{
	uint16_t x = 12345;
	uint8_t p = 7;
	uint8_t n = 4;
	uint8_t y = 12;
	uint16_t z;

	z = setbits(x, p, n, y);
	printf("getbits(%u %x), %d, %d) = %u (%x)\n", x, x, p, n, z, z);
	if(z == 9785)
		puts("Woohooo");
}

/*
 * Setbits 'n' from position 'x' to position 'y'.
 */
static uint16_t setbits(uint16_t x, uint8_t p, uint8_t n, uint8_t y)
{
	uint16_t x1;
	uint16_t x2;

	x1 = (x >> (p+1-n) & ~(~0 << n)) << (y+1-n);
	x2 = x & ~((~(~0 << n)) << (y+1-n));

	return x2 | x1;
}

