/*
 * i < lim-1 && (c = getchar()) != '\n' && c != EOF
 *
 * Exercise 2-2. Write a loop equivalent to the for loop above without using &&
 * or ||.
 */
#include <stdio.h>

#define MAXLINE		1000

int main(void)
{
	int i;
	int c;
	int lim = MAXLINE;

	for (i = 0; i < lim-1; i++)
	{
		if ((c = getchar()) != '\n')
		{
			if (c != EOF)
			{
				putchar(c);
			}
			else 
			{
				break;
			}
		}
		else 
		{
			break;
		}
	}
}
