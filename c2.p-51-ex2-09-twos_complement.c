/*
 * Exercise 2-9. In a two's complement number system, x &= (x-1) deletes the
 * rightmost 1-bit in x. Explain why. Use this observation to write a faster
 * version of bitcount.
 *
 * The rightmost bit is deleted due to the power of 2, its being inherent in
 * the binary system. As the integer is diminished and the & operator then
 * applied to the result, the least significant 1 bit is changed into a 0 and
 * the least significant 0 to a 1, when the & operator is then applied the net
 * result is the cancellation of both of the 1's.
 */

#include <stdio.h>

/* 
 * Original bitcount: count 1 bits in x
 * When the least significant bit is compaired with 01, if it is 1 the
 * condition is true, if not then 0 is & with 01 and thus false.
 */

static int bitcount(int x)
{
	int b;

	for (b = 0; x != 0; x >>= 1)
		if (x & 01)
			b++;
	return b;
}

/*
 * The improved faster bitcount uses &= x-1 to remove the least significnt bit
 * and reduce the value of x by the most prominante value of its own construct
 * in powers of 2, corresponding to that bit.
 */

static int fasterBitcount(int x)
{
	int b;

	for (b = 0; x != 0; x &= (x-1))
		b++;

	return b;
}

int main(void)
{
	int x;

	puts("Enter a value of x:");
	scanf("%d", &x);
	fasterBitcount(x);

	return 0;
}

