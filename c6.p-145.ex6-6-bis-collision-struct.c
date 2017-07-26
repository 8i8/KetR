/*
 * This program has been written as a tool for testing hash algorithms. Though
 * the primary motivation has been to understand the kind of recursive function
 * that can be used to solve the towers of Hanoi type problem.
 *
 * TODO The recursive function that walks the string of elements augmenting
 * when required, can likely be made much more efficient using pointer
 * arithmetic.
 */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define DEBUG		0
#define SHOW		0
#define MAX_IN		1000
#define MAX_OUT		100

enum state { RUN, AUG, FIN, ERR }; 

struct D {
	char s[MAX_IN];		/* string to be examined */
	char sr[MAX_OUT];	/* output for hash as text */
	unsigned long hash;	/* hash value */
	size_t nel;		/* number  of elements */
	size_t ofs;		/* char offset */
	size_t pos;		/* current position on the array */
	size_t p_ofs;		/* istart position offste on the array */
	size_t len;		/* array length */
	short e;		/* status */
	struct D *link;		/* the other one */
};

typedef struct D Data;

static void utoa(unsigned long n, char s[]);
static Data placecount(Data data, size_t pos);
static Data unitcount(Data data);
static Data findhash(Data data);
static void print(Data data, char *s, short clear);
static char* __getline(char *s, size_t lim);

#define P1 	317			/* prime smaller than s^16/200
					   (unlikely to use more than 200 elements) */
#define P2 	65521			/* prime smaller than 2^16 */
#define P3	281539415969051		/* prime smaller than 2^64 / 65521 */

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
			printf("-> %lu\t", hashval);
			printf("-> %lu\n", hashval % P2);
		}
	}
	return hashval % P2;
}

int main(int argc, char* argv[])
{
	Data input = { {'\0'}, {'\0'}, 0, 0, 0, 0, 0, MAX_OUT, 0, 0 };
	Data comp = { {'\0'}, {'\0'}, 0, 0, 0, 0, 0, MAX_OUT, 0, 0 };

	char c;
	/* The printable charater set */
	comp.ofs = 33;
	comp.nel = 94;

	while (--argc > 0 && (*++argv)[0] == '-')
		while ((c = *++argv[0]))
			switch (c) {
				case 'b':
					/* Binary */
					comp.ofs = 48;
					comp.nel = 2;
					break;
				case 'a':
					/* Letters */
					comp.ofs = 97;
					comp.nel = 27;
					break;
				case 'p':
					/* The printable charater set */
					comp.ofs = 33;
					comp.nel = 94;
					break;
				case 't':
					strcpy(input.s, "Find a string with a similar hash to that of this sentence ...");
					strcpy(comp.s, "Another line just to test ... ");
					comp.p_ofs = comp.pos = strlen(comp.s);
					break;
				case 'n':
					/* Numbers */
					comp.ofs = 48;
					comp.nel = 10;
				default:
					break;
			}

	input.link = &comp;
	comp.link = &input;

	if (!strlen(input.s))
		__getline(input.s, MAX_IN);

	input.hash = hash(input.s, SHOW);
	utoa(input.hash, input.sr);

	comp = findhash(comp);

	comp.hash = hash(comp.s, SHOW);
	utoa(comp.hash, comp.sr);

	if(comp.e == FIN) {
		print(input,"Input", 0);
		print(comp, "Output", 0);
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
static Data placecount(Data data, size_t pos)
{
	data.e = RUN;
	/*
	 * len-1 as this check is always performed on the second column never the units.
	 */
	if (pos > data.len)
		data.e = ERR, write(1, "error:	data overflow; Increase max data.length.\n", 51);
	/*
	 * If the character is not the upper most, augment pos.
	 */
	else if (data.s[pos] < (int)(data.nel + data.ofs-1))   
		data.s[pos]++;
	/*
	 * It is time to move to next data value.
	 */
	else if (pos > data.p_ofs) {
		data.s[pos+1] = data.ofs;
		data.s[pos] = data.ofs;
		data = placecount(data, --pos);
	/*
	 * The data value has reached the current maximum, time to add a column or to end.
	 */
	} else if (pos == data.p_ofs) {
		if ((strlen(data.s) < data.len)) {
			data.s[pos] = data.ofs+1;
			data.s[pos+1] = data.ofs;
			data.e = AUG;
		} else
			data.e = ERR, write(1, "error:	data overflow; Increase max data.length.\n", 51);
	}

	return data;
}

/*
 * unitcount:	Iterate through nel units incrementally.
 */
static Data unitcount(Data data)
{
	size_t i;
	data.e = RUN;

	if (data.pos+data.p_ofs >= data.len) {
		write(1, "error:	overflow in unitcount()\n", 33);
		data.e = ERR;
		return data;
	} 

	/*
	 * Iterate through all numbers.
	 */
	for (i = 0; i < data.nel && data.pos+data.p_ofs < data.len; i++) {
		data.s[data.pos+data.p_ofs] = data.ofs + i;
		/*
		 * Test for match.
		 */
		if (((data.hash = hash(data.s, 0)) == data.link->hash)) {
			data.e = FIN;
			if (SHOW) {
				printf("in loop input -> %lu\n", data.link->hash);
				printf("in loop comp -> %lu\n", data.hash);
			}
			return data;
		}
		if (DEBUG)
			print(data,"Debug", 1);
	}

	return data;
}

/*
 * findhash:    Combine the two function unitcount and placecount(), to findhash
 *              through all possible combinations of the units provided.
 */
static Data findhash(Data data)
{
	data.e = RUN;

	data = unitcount(data);

	if (data.len > 1)
		data.s[data.pos++] = data.ofs+1;

	while (data.pos+data.p_ofs < data.len && data.e < FIN) {
		data = unitcount(data);
		if(data.e < FIN)
			data = placecount(data, data.pos-1);
		if (data.e == AUG) {
			data.pos++;
			data.s[data.pos+1] = '\0';
		}
	}

	return  data;
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
 *  I/O
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
/*
 * print:	Print the struct provided, 'clear' specifies whether or not to
 *  		refresh the screen.
 */
static void print(Data data, char *s, short clear)
{
		if (clear)
			puts("\033[H\033[J");
		write(1, s, strlen(s)); 
		write(1, " -> ", 4); 
		write(1, "`", 1);
		write(1, data.s, strlen(data.s));
		write(1, "`", 1);
		write(1, " :~ ", 4);
		write(1, data.sr, strlen(data.sr));
		write(1, "\n", 1);
}

/*
 * getline:	input text
 */
static char* __getline(char *s, size_t lim)
{
	char c;
	char *pt;
	pt = s;

	while(lim-- > 0 && ((c = getchar()) != EOF))
			if (c != '\n')
				*s++ = c;
	*s = '\0';

	return pt;
}

