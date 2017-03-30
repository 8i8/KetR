/*
 * Exercise 2-10. Rewrite the function lower, which converts upper case letters
 * to lower case, with a conditional expression instead of if-else.
 */
#include <stdio.h>
#include <stdint.h>

/* lower: convert c to lower case; ASCII only */
static int8_t lower(int8_t c)
{
	if (c >= 'A' && c <= 'Z')
		return c + 'a' - 'A';
	else
		return c;
}

static int8_t lowerConditional(int8_t c)
{
	return(c >= 'A' && c <= 'Z') ? c + 'a' - 'A' : c;
}

int main(void)
{
	int8_t c;
	int8_t x;

	while((c = getchar()) != EOF)
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

