#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define DEBUG		0

#define MAX_IN		1000
#define MAX_OUT		10	

enum state { RUN, AUG, FIN, ERR }; 

static void utoa(size_t n, char s[]);
static char* iterate(unsigned h1, char *s, size_t ofs, size_t nel, size_t len);

#define P1 	31
#define P2 	62
#define P3 	101
#define P4 	211
#define P5 	237
#define P6 	251
#define P7 	1949
#define P8 	65521
#define P9 	92821
#define P10	131071
#define P11	262139
#define P12	524287
#define P13	900001

/*
 * hash:	form hash value for string s
 */
static unsigned hash(char *s)
{
	unsigned long hashval;

	for (hashval = 0; *s != '\0'; s++)
		hashval = *s + P12 * hashval;
	return hashval % P8;
}

//static unsigned hash(char *str)
//{
//	unsigned char *ucp; // changed from char
//	unsigned long int   u;
//	unsigned long int   g;
//	unsigned long int   h = 0;
//
//	for (ucp = (unsigned char *)str ; (*ucp) ; ucp++) {
//		u = *ucp;
//		h <<= 4;
//		h += u;
//		g = h & 0xf0000000U;
//		/* branch eliminated */
//		h ^= (g >> 24);
//		h ^= g;
//	}
//
//	return h % P8;
//}

int main(void)
{
	char s1[MAX_IN];
	char s2[MAX_OUT];
	char h1s[MAX_OUT];
	unsigned h1;
	h1 = 0;

	strcpy(s1, "Find a string with a similar hash to that of this sentance ...");
	//strcpy(s1, "Another line just to test ...");

	h1 = hash(s1);
	utoa(h1, h1s);

	//puts("\033[H\033[J");	/* Clear screen */
	write(1, "input -> ", 10);
	write(1, s1, strlen(s1)+1);
	write(1, " :~ ", 5);
	write(1, h1s, strlen(h1s)+1);
	write(1, "\n", 2);

	iterate(h1, s2, 33, 94, MAX_OUT);
	//iterate(h1, s2, 48, 10, MAX_OUT);

	return 0;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Calc
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */

/*
 * counter:	count with given radix, radix = nel+1. 
 * 		ofs being the offset to display which ever charter is desired.
 */
static int placecount(char *s, size_t ofs, size_t nel, size_t pos, size_t len)
{
	short e = RUN;

	/*
	 * len-1 as this check is always peformed on the second collumn never the units.
	 */
	if (pos >= len-1)
		e = ERR, write(1, "error:	place overflow; Increase max length.\n", 33);
	/*
	 * If the character is not the upper most, augment pos.
	 */
	else if (s[pos] < (int)(nel+ofs-1))   
		s[pos]++;
	/*
	 * It is time to move to next place value.
	 */
	else if (pos > 0) {
		s[pos+1] = ofs;
		s[pos] = ofs;
		e = placecount(s, ofs, nel, --pos, len);
	/*
	 * The place value has reached the current maximum, time to add a collumn or to end.
	 */
	} else if (pos == 0) {
		if (!(strlen(s) == len))
			s[pos] = ofs+1, s[pos+1] = ofs, e = AUG;
		else
			e = ERR, write(1, "error:	place overflow; Increase max length.\n", 33);
	}

	return e;
}

/*
 * unitcounter:	Iterate through nel units incrimentaly.
 */
static int unitcount(unsigned h1, char *s, size_t ofs, size_t nel, size_t pos, size_t len)
{
	unsigned h2;
	size_t i;
	short e;
	char str[len];
	e = 0;

	if (pos >= len) {
		write(1, "error:	overflow in unitcount()\n", 33);
		return ERR;
	}
	/*
	 * Iterate through all numbers.
	 */
	for (i = 0; i < nel && pos < len; i++) {
		s[pos] = i+ofs;
		/*
		 * Test for match.
		 */
		if ((h2 = hash(s)) == h1) {
			e = FIN;
			utoa(h2, str);
			write(1, "match -> ", 9); 
			write(1, "\"", 2);
			write(1, s, strlen(s)+1);
			write(1, "\"", 2);
			write(1, " :~ ", 5);
			write(1, str, strlen(str)+1);
			write(1, "\n", 2);
			break;
		}
		if (DEBUG) {
			utoa(h2, str);
			puts("\033[H\033[J");
			write(1, "debug match -> ", 15); 
			write(1, "\"", 2);
			write(1, s, strlen(s)+1);
			write(1, "\"", 2);
			write(1, " :~ ", 5);
			write(1, str, strlen(str)+1);
			write(1, "\n", 2);
		}
	}

	return e;
}

/*
 * iterate:     Combine the two function unitcount and placecount(), to iterate
 *              through all posible combinations of the units provided.
 */
static char* iterate(unsigned h1, char *s, size_t ofs, size_t nel, size_t len)
{
	size_t pos;
	short e, m;
	pos = e = m = 0;

	e = unitcount(h1, s, ofs, nel, pos, len);

	if (len > 1)
		s[pos++] = ofs+1;

	while (pos < len && e < FIN) {
		e = unitcount(h1, s, ofs, nel, pos, len);
		if(e < FIN)
			e = placecount(s, ofs, nel, pos-1, len);
		if (e == AUG)
			++pos;
	}

	switch (e) {
		case FIN:
			write(1, "Fin", 4);
			write(1, "\n", 2);
			break;
		case ERR:
			write(1, "Error: ", 8);
			write(1, "\n", 2);
			break;
		default:
			write(1, "Default reached in switch", 26);
			write(1, "\n", 2);
	}

	return  s;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  utoa  
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
static void __reverse(char s1[]);

/*
 * Transform a value of the int type into a string.
 */ 
static void utoa(size_t n, char s[])
{
	size_t i;

	i = 0;
	do {
		/* make n positive and generate digits in reverse order */
		s[i++] = (char)(n % 10) + '0';
	}
	while ((n /= 10) != 0);

	s[i] = '\0';

	__reverse(s);
}

/*
 * Reverse the array.
 */
static void __reverse(char s1[])
{
	size_t i, j;
	int8_t c;

	for (i = 0, j = strlen(s1) - 1; i < j; i++, j--) {
		c = s1[j];
		s1[j] = s1[i];
		s1[i] = c;
	}
}

