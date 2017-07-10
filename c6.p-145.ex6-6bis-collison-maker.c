#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASHSIZE	101

/*
 * hash:	form hash value for string s
 */
static unsigned hash(char *s)
{
	unsigned hashval;

	for (hashval = 0; *s != '\0'; s++)
		hashval = *s + 31 * hashval;
	return hashval % HASHSIZE;
}

int main(void)
{
	char *s1 = "HELLO";
	char string[100] = "HELLO";
	char *s2;
	char add[2];
	int c = 'A';
	s2 = string;


	unsigned h1, h2;

	h1 = hash(s1);
	h2 = hash(s2);

	do {
		strcpy(string, "HELLO");
		add[0] = c;
		s2 = strcat(s2, add);
		c++;
	} while ((h2 = hash(s2)) != h1 );

	h1 = hash(s1);
	h2 = hash(s2);

	printf("%s hash -> %d\n", s1, h1);
	printf("%s hash -> %d\n", s2, h2);

	return 0;
}

