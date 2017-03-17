/*
 * Excersise 1-5
 *
 * Modify the temperature conversion program to print the table in the reverse
 * order, that is from 300 degrees to 0.
 */

#include <stdio.h>

int main(int argc, char *argv[])
{
	float fahr, celsius;
	int lower, upper, step;

	lower = 0;
	upper = 300;
	step  = 20;

	printf("Celcius    Fahr\n");
	printf("~~~~~~~~~~~~~~~\n");
	for (fahr = upper; fahr >= lower; fahr = fahr - step)
		printf("%3d\t %6.1f\n", fahr, (5.0 / 9.0)*(fahr - 32));
}
