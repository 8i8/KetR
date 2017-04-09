/*
 * Exercise 4-14. Define a macro swap(t,x,y) that interchanges two arguments of
 * type t. (Block structure will help.)
 */
#include <stdio.h>

#define swap(t, x, y) 	{	\
	t temp;			\
	temp = x;		\
	x = y;			\
	y = temp;		\
}

int main(void)
{
	int i = 1;
	int j = 2;

	swap(int, i, j);

	printf("i = %d\nj = %d\n", i, j);
}

