#include <stdio.h>

int main(void)
{
	int i;

	for (i = 1; i < 101; i++) {
		if (i%3 == 0 && i%5 != 0)
			puts("Fizz");
		else if (i%3 != 0 && i%5 == 0)
			puts("Buzz");
		else if (i%3 == 0 && i%5 == 0)
			puts("FizzBuzz");
		else
			printf("%d\n", i);
	}

	return 0;
}

