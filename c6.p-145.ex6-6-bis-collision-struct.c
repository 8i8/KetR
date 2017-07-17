#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define DEBUG		0
#define SHOW		0
#define MAX_IN		1000
#define MAX_OUT		10	

enum state { RUN, AUG, FIN, ERR }; 

struct D {
	char s[MAX_IN];		/* string to be examined */
	char sr[MAX_OUT];	/* output for hash as text */
	unsigned long hash;	/* hash value */
	size_t ofs;		/* char offset */
	size_t nel;		/* number  of elements */
	size_t pos;		/* current position on the array */
	size_t len;		/* array length */
	short e;		/* status */
	struct D *link;		/* the other one */
};

typedef struct D Data;

static void utoa(unsigned long n, char s[]);
static Data placecount(Data place, size_t pos);
static Data unitcount(Data unit);
static Data findhash(Data iter);
static void print(Data pr, short clear);

#define P1 	317			/* prime just smaller than s^16/200
					   (unlikely to use more than 200 elements) */
#define P2 	65521			/* prime just smaller than 2^16 */
#define P3	281539415969051		/* prime just smaller than 2^64 / 65521 */

/*
 * hash:	form hash value for string s
 */
static unsigned long hash(char *s, short show)
{
	unsigned long hashval;

	for (hashval = 0; *s != '\0'; s++) {
		hashval = *s + P1 * hashval;
		/* A prime is used for a modulus to avoid wrapping, although
		 * this is not a problem for unsigned int, it is I think more
		 * likely to form a patten if it is not prime. */
		hashval %= P3;
		if (show) {
			printf(" -> %lu\t", hashval);
			printf("-> %lu\n", hashval % P2);
		}
	}
	return hashval % P2;
}

int main(void)
{
	Data input;
	Data comp;

	input.link = &comp;
	comp.link = &input;

	strcpy(input.s, "Find a string with a similar hash to that of this sentence ...");
	//strcpy(input.s, "Another line just to test ...");

	/* Numbers only */
	//comp.ofs = 48;
	//comp.nel = 10;

	/* The entire alphabet */
	comp.ofs = 33;
	comp.nel = 94;

	input.len = MAX_OUT;
	comp.len = MAX_OUT;

	input.hash = hash(input.s, SHOW);
	utoa(input.hash, input.sr);

	comp = findhash(comp);

	comp.hash = hash(comp.s, SHOW);
	utoa(comp.hash, comp.sr);

	if(comp.e == FIN) {
		print(input, 0);
		print(comp, 0);
	}

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
static Data placecount(Data place, size_t pos)
{
	place.e = RUN;
	/*
	 * len-1 as this check is always performed on the second column never the units.
	 */
	if (pos >= place.len-1)
		place.e = ERR, write(1, "error:	place overflow; Increase max place.length.\n", 51);
	/*
	 * If the character is not the upper most, augment pos.
	 */
	else if (place.s[pos] < (int)(place.nel + place.ofs-1))   
		place.s[pos]++;
	/*
	 * It is time to move to next place value.
	 */
	else if (pos > 0) {
		place.s[pos+1] = place.ofs;
		place.s[pos] = place.ofs;
		place = placecount(place, --pos);
	/*
	 * The place value has reached the current maximum, time to add a column or to end.
	 */
	} else if (pos == 0) {
		if (!(strlen(place.s) == place.len))
			place.s[pos] = place.ofs+1, place.s[pos+1] = place.ofs, place.e = AUG;
		else
			place.e = ERR, write(1, "error:	place overflow; Increase max place.length.\n", 51);
	}

	return place;
}

/*
 * unitcounter:	Iterate through nel units incrementally.
 */
static Data unitcount(Data unit)
{
	size_t i;
	unit.e = RUN;

	if (unit.pos >= unit.len) {
		write(1, "error:	overflow in unitcount()\n", 33);
		unit.e = ERR;
		return unit;
	} 

	/*
	 * Iterate through all numbers.
	 */
	for (i = 0; i < unit.nel && unit.pos < unit.len; i++) {
		unit.s[unit.pos] = unit.ofs + i;
		/*
		 * Test for match.
		 */
		if ((unit.hash = hash(unit.s, 0) == unit.link->hash)) {
			unit.e = FIN;
			if (SHOW) {
				printf("in loop input -> %lu\n", unit.link->hash);
				printf("in loop comp -> %lu\n", unit.hash);
			}
			return unit;
		}
		if (DEBUG)
			print(unit, 1);
	}

	return unit;
}

/*
 * findhash:    Combine the two function unitcount and placecount(), to findhash
 *              through all possible combinations of the units provided.
 */
static Data findhash(Data ite)
{
	ite.pos = ite.e = RUN;

	ite = unitcount(ite);

	if (ite.len > 1)
		ite.s[ite.pos++] = ite.ofs+1;

	while (ite.pos < ite.len && ite.e < FIN) {
		ite = unitcount(ite);
		if(ite.e < FIN)
			ite = placecount(ite, ite.pos-1);
		if (ite.e == AUG)
			ite.pos++;
	}

	return  ite;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  utoa  
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
static void __reverse(char s1[]);

/*
 * Transform a value of the unsigned int type into a string.
 */ 
static void utoa(unsigned long n, char s[])
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

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Print
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
/*
 * print:	Print the struct provided, 'clear' specifies wheter or not to
 *  		refresh the screen.
 */
static void print(Data pr, short clear)
{
		if (clear)
			puts("\033[H\033[J");
		write(1, "match -> ", 9); 
		write(1, "`", 2);
		write(1, pr.s, strlen(pr.s)+1);
		write(1, "`", 2);
		write(1, " :~ ", 5);
		write(1, pr.sr, strlen(pr.sr)+1);
		write(1, "\n", 2);
}
