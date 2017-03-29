/*
 * Exercise 2-7. Write a function invert(x,p,n) that returns x with the n bits
 * that begin at position p inverted (i.e., 1 changed into 0 and vice versa),
 * leaving the others unchanged.
 */
#include <stdio.h>
#include <stdint.h>

static uint16_t invert(uint16_t x, uint8_t p, uint8_t n)
{
	uint16_t mask;
	uint16_t sect;

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
	uint16_t x = 12345;
	uint8_t p = 9;
	uint8_t n = 5;
	uint16_t y;

	y = invert(x, p, n);
	printf("getbits(%u %x), %d, %d) = %u (%x)\n", x, x, p, n, y, y);
	if(y == 13273)
		puts("Woohooo");
}

