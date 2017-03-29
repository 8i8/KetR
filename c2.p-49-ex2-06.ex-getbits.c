/*
 *  	Let's use 16 bits for our example. In that case, ~0 is equal to
 *
 *  	1111111111111111
 *
 *  	When we left-shift this n bits (3 in your case), we get:
 *
 *  	1111111111111000
 *
 *  	because the 1s at the left are discarded and 0s are fed in at the
 *  	right. Then re-complementing it gives:
 *
 *  	0000000000000111
 *
 *  	so it's just a clever way to get n 1-bits in the least significant part
 *  	of the number.
 *
 *  	x = (x >> (p+1-n)) & ~(~0 << n);
 *
 *  	The "x bit" you describe has shifted the given number (f994) right far
 *  	enough so that the least significant 3 bits are the ones you want. In
 *  	this example, the bits you're requesting are surrounded by '.'
 *  	characters.
 *
 *  	int p = 4;
 *  	int n = 3;
 *
 *  	ff94             11111111100.101.00  # original number
 *  	>> p+1-n     [2] 0011111111100.101.  # shift desired bits to right
 *  	& ~(~0 << n) [7] 0000000000000.101.  # clear all the other (left) bits
 *
 *
 *	~(~0 << n) creates a mask that will have the n right-most bits turned on.
 *	
 *	0
 *	   0000000000000000
 *	~0
 *	   1111111111111111
 *	~0 << 4
 *	   1111111111110000
 *	~(~0 << 4)
 *	   0000000000001111
 *	
 *	ANDing the result with something else will return what's in those n bits.
 */
#include <stdio.h>
#include <stdint.h>

static uint8_t getbits(uint8_t x, uint8_t p, uint8_t n)
{
	return (x >> (p + 1 - n)) & ~(~0 << n);
}

int main(void)
{
	uint16_t x = 0xF994;
	uint8_t p = 4;
	uint8_t n = 3;
	uint8_t z = 0;

	z = getbits(x, p, n);

	printf("getbits(%u %x), %d, %d) = %u (%x)\n", x, x, p, n, z, z);

	return 0;
}

