/*
 * Exercsise 1-6. Verify that the expression getchar() != EOF is 0 or 1.
 *
 * c = getchar() != EOF;
 *
 *  is equivalent to:
 *
 * c = (getchar() != EOF);
 *
 * This have the undesired effect of setting c to be either 1 or 0 depending
 * upon whether the EOF has been reached.
 */
#include <stdio.h>

int main(void)
{
	int c;

	printf("Press any key and then <ENTER> to run getchar()");
	c = (getchar() != EOF);
	printf("c = %d\n", c);

	return 0;
}
