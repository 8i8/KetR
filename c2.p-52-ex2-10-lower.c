/*
 * Exercise 2-10. Rewrite the function lower, which converts upper case letters
 * to lower case, with a conditional expression instead of if-else.
 */
#include <stdio.h>

/* lower: convert c to lower case; ASCII only */
int lower(int c)
{
	if (c >= 'A' && c <= 'Z')
		return c + 'a' - 'A';
	else
		return c;
}

int lowerConditional(int c)
{
	return(c >= 'A' && c <= 'Z') ? c + 'a' - 'A' : c;
}

int main(void)
{
	int c;
	int x;

	while((c = getchar()) != 'Q')
	{
		if(c != '\n') {
			printf("if else : ");
			x = lower(c);
			putchar(x);
			puts("");
			printf("conditional : ");
			x = lowerConditional(c);
			putchar(x);
			puts("");
		}
	}
}

