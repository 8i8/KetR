/*
 * Exercise 1-4. Write a program to print the corresponding Celsius to Fahrenheit table.
 */
#include <stdio.h>

/*
 * Print out Fahrenheit- Celsius using floating-point precision.
 */
int main(int argc, char *argv[])
{
	float fahr, celsius;
	int lower, upper, step;

	lower = 0;
	upper = 300;
	step  = 20;

	celsius = lower;
	printf("Celcius     Fahr\n");
	printf("~~~~~~~~~~~~~~~~\n");
	while (celsius <= upper) {
		fahr = (celsius+32.0) / (5.0 / 9.0);
		printf("%3.0f %12.1f\n", celsius, fahr);
		celsius = celsius + step;
	}
}

