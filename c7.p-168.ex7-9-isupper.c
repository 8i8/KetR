/*
 * Exercise 7-9. Functions like isupper can be implemented to save space or to
 * save time. Explore both possibilities.
 */
#include <stdio.h>

/**
 * mac_isupper:	The macro version of isupper will run faster as there is no
 * function call, however it is written out in full every time that it is used
 * and as such can take up more space.
 */
#define mac_isupper(c)  ((c) >= 'A' && (c) <= 'Z') ? 1 : 0

/**
 * func_isupper:	Is upper as a function uses less space than it would as
 * a macro, however it may be slighltly less efficeient as it will use a
 * function call.
 */
int func_isupper(char c)
{
	if (c >= 'A' && c <= 'Z')
		return 1;
	else
		return 0;
}

int main(void)
{
	char a = 'A';
	char b = 'b';

	if (func_isupper(a))
		printf("%c is uppercase.\n", a);
	else
		printf("%c is lowercase.\n", a);

	if (mac_isupper(b))
		printf("%c is uppercase.\n", b);
	else
		printf("%c is lowercase.\n", b);

	return 0;
}
