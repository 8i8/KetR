/*
 * c2.03.3-58-ex3-01-binary_search.c
 *
 * Exercise 3-1. Our binary search makes two tests inside the loop, when one
 * would suffice (at the price of more tests outside.) Write a version with
 * only one test inside the loop and measure the difference in run- time.
 *
 * This program generates a random number between the defined limits and then
 * an array with that amount of cells. The array is then filled with random
 * values and sorted into ascending order; Passed through three different
 * search functions, each of which is timed.
 *
 * This code uses the <time.h> header to time functions at the lowest system
 * level thus the highest resolution.
 *
 * <time.h>
 *
 * typedef __clockid_t clockid_t;
 * extern int clock_gettime (clockid_t __clock_id, struct timespec *__tp) __THROW;
 *
 * https://linux.die.net/man/3/clock_getres
 * http://pubs.opengroup.org/onlinepubs/9699919799/functions/clock_gettime.html
 * https://www.cs.rutgers.edu/~pxk/416/notes/c-tutorials/gettime.html
 */
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdint.h>

#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // equivalent to (K&R) #include "syscalls.h"

#define BILLION		1000000000L
#define A_MAX		10000
#define A_MIN		1

typedef uint_fast16_t (*search_fn)(	const uint_fast16_t,
					const uint_fast16_t*,
					const uint_fast16_t);

/*
 * This algorithm contains a bug and will go into an infinite loop under
 * certain conditions.
 */
/* binsearch: find x in v[0] <= v[1] <= ... <= v[n-1] */
static uint_fast16_t binsearch(		const uint_fast16_t x,
					const uint_fast16_t v[],
					const uint_fast16_t n)
{
	uint_fast16_t low, high, mid;
	low = 0;
	high = n - 1;
	while (low <= high) {

		mid = (low+high)/2;
		if (x < v[mid])
			high = mid - 1;
		else if (x > v[mid])
			low = mid + 1;
		else
			/* found match */
			return mid;
	}
	/* no match */
	printf("n/a");
	return -1;
}

/*
 * A different order ...
 */
static uint_fast16_t binarySearchOne(	const uint_fast16_t x,
					const uint_fast16_t v[],
					const uint_fast16_t n)
{
	uint_fast16_t low, high, mid;
	low = 0;
	high = n - 1;
	mid = (low+high)/2;
	while (low < high && x != v[mid]) {

		if (x > v[mid])
			low = mid + 1;
		else
			high = mid - 1;
		mid = (low + high)/2;
	}
	if (x == v[mid])
		return mid;
	/* no match */
	printf("n/a");
	return -1;
}

/*
 * Optimised, only one comparison made per iteration instead of two.
 */
static uint_fast16_t binarySearchTwo(	const uint_fast16_t x,
					const uint_fast16_t v[],
					const uint_fast16_t n)
{
	register uint_fast16_t low, high, mid;
	low = 0;
	high = n - 1;
	mid = (low+high)/2;
	while (low < high && x != v[mid]) {

		if (x > v[mid])
			low = mid + 1;
		else
			high = mid - 1;
		mid = (low + high)/2;
	}
	if (x == v[mid])
		return mid;
	/* no match */
	printf("n/a");
	return -1;
}

/*
 * Runs an empty search call, for comparison.
 */
static uint_fast16_t noSearch(		const uint_fast16_t x,
					const uint_fast16_t v[],
					const uint_fast16_t n)
{
	return -1;
}

/*
 * Return a random value between the given limits, I have used timespec to
 * generate the seed for my random value for this reason the program will only
 * run on a POSIX machine, I shall look further into this subject in the future
 * as it is clearly a facinating area of the computing landscape.
 */
static uint_fast16_t myRand(uint_fast16_t min, uint_fast16_t max)
{
	struct timespec seed;
	clock_gettime(CLOCK_MONOTONIC, &seed);
	int range;
	int x;

	range = max - min;

	if(min > max) {
		x = min;
		min = max;
		max = x;
	}

	srand(seed.tv_nsec);
	x = rand()%range+1;

	return x;
}

/*
 * Fill array with randomly generated values.
 */
static void fillArray(uint_fast16_t array[], const uint_fast16_t len)
{
	size_t i;
	for(i = 0; i < len; i++)
		array[i] = myRand(A_MIN, A_MAX);	
}

/*
 * Sort the array, ascending.
 */
static void sortArray(uint_fast16_t array[], const uint_fast16_t len)
{
	size_t i;
	uint_fast16_t temp;
	uint8_t isMod;

	do
	{
		isMod = 0;
		for (i = 1; i < len; i++)
		{
			if (array[i-1] > array[i])
			{
				temp = array[i-1];	
				array[i-1] = array[i];
				array[i] = temp;
				isMod = 1;
			}
		}
	}
	while(isMod);
}

/*
 * Run the provided search function whilst timing the operation.
 * CLOCK_MONOTONIC time is the total time and CLOCK_PROCESS_CPUTIME_ID is the
 * time for the individual process.
 */
static uint64_t timeIt(	const search_fn fn,
			const uint_fast16_t x,
			uint_fast16_t v[],
			const uint_fast16_t n,
			const uint8_t time_method)
{
	struct timespec start, end;
	size_t i;
	uint64_t time;
	uint64_t factor;

	factor = 5000000;
	time = 0;

	for (i = 0; i < factor; i++) {
		clock_gettime(time_method, &start);
		(*fn)(x, v, n);
		clock_gettime(time_method, &end);
		time += BILLION * (end.tv_sec - start.tv_sec)
					+ end.tv_nsec - start.tv_nsec;
	}

	time /= factor;

	return time;
}

int main(void)
{
	int8_t c;
	uint64_t time;

	puts("Press a key to start ...");

	while((c = getchar()) != EOF)
	{
		uint_fast16_t n = myRand(A_MIN, A_MAX);
		uint_fast16_t x = myRand(A_MIN, n);
		uint_fast16_t v[n];
		/* */
		fillArray(v, n);
		x = v[x];
		sortArray(v, n);

		search_fn searchOriginal;
		search_fn searchOne;
		search_fn searchTwo;
		/* */
		searchOriginal = binsearch;
		searchOne = binarySearchOne;
		searchTwo = binarySearchTwo;

		/*
		 * CLOCK_MONOTONIC
		 * CLOCK_MONOTONIC_RAW
		 * CLOCK_PROCESS_CPUTIME_ID
		 */
		time = timeIt(noSearch, x, v, n, CLOCK_PROCESS_CPUTIME_ID);
		printf("Time for no search :~ %5lu\n", time);
		/* */
		puts("");

		time = timeIt(searchOriginal, x, v, n, CLOCK_PROCESS_CPUTIME_ID);
		printf("Time for Orig :~ %5lu\n", time);
		/* */
		time = timeIt(searchOriginal, x, v, n, CLOCK_PROCESS_CPUTIME_ID);
		printf("Time for Orig :~ %5lu\n", time);
		/* */
		time = timeIt(searchOriginal, x, v, n, CLOCK_PROCESS_CPUTIME_ID);
		printf("Time for Orig :~ %5lu\n", time);
		/* */
		puts("");

		time = timeIt(searchTwo, x, v, n, CLOCK_PROCESS_CPUTIME_ID);
		printf("Time for Thir :~ %5lu\n", time);
		/* */
		time = timeIt(searchTwo, x, v, n, CLOCK_PROCESS_CPUTIME_ID);
		printf("Time for Thir :~ %5lu\n", time);
		/* */
		time = timeIt(searchTwo, x, v, n, CLOCK_PROCESS_CPUTIME_ID);
		printf("Time for Thir :~ %5lu\n", time);
		/* */
		puts("");

		time = timeIt(searchOne, x, v, n, CLOCK_PROCESS_CPUTIME_ID);
		printf("Time for Seco :~ %5lu\n", time);
		/* */
		time = timeIt(searchOne, x, v, n, CLOCK_PROCESS_CPUTIME_ID);
		printf("Time for Seco :~ %5lu\n", time);
		/* */
		time = timeIt(searchOne, x, v, n, CLOCK_PROCESS_CPUTIME_ID);
		printf("Time for Seco :~ %5lu\n", time);
		/* */
		puts("");

	}
	return 0;
}

