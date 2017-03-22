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

#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // equivalent to (K&R) #include "syscalls.h"

#define BILLION		1000000000L
#define A_LENGTH	10000
#define A_MIN		1

typedef int (*search_fn)(int, int*, int);

/*
 * This algorithm contains a bug and will go into an infinite loop under
 * certain conditions.
 */
/* binsearch: find x in v[0] <= v[1] <= ... <= v[n-1] */
int binsearch(int x, int v[], int n)
{
	int low, high, mid;
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
int binarySearchOne(int x, int v[], int n)
{
	int low, high, mid;
	low = 0;
	high = n - 1;
	mid = (low+high)/2;

	while (low <= high) {

		if (x > v[mid])
			low = mid + 1;
		else if (x == v[mid])
			return mid;
		else
			high = mid - 1;

		mid = (low+high)/2;
	}
	/* no match */
	printf("n/a");
	return -1;
}

/*
 * Optimised, only one comparison made per iteration instead of two.
 */
int binarySearchTwo(int x, int v[], int n)
{
	int low, high, mid;
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
int noSearch(int x, int v[], int n)
{
	return -1;
}

/*
 * Return a random value between the given limits, I have used timespec to
 * generate the seed for my random value for this reason the program will only
 * run on a POSIX machine, I shall look further into this subject in the future
 * as it is clearly a facinating area of the computing landscape.
 */
int myRand(int min, int max)
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
void fillArray(int array[], int len)
{
	int i;
	for(i = 0; i < len; i++)
		array[i] = myRand(A_MIN, A_LENGTH);	
}

/*
 * Sort the array, ascending.
 */
void sortArray(int array[], int len)
{
	int i;
	int temp;
	int isMod;

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
unsigned long long timeIt(search_fn fn, int x, int v[], int n, int time_method)
{
	size_t i;
	struct timespec start, end;
	unsigned long long time;
	unsigned long long factor;

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
	int c;
	unsigned long long time;

	puts("Press a key to start ...");

	while((c = getchar()) != 'q')
	{
		int n = myRand(A_MIN, A_LENGTH);
		int x = myRand(A_MIN, n);
		int v[n];
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
		printf("Time for no search :~ %5llu\n", time);
		/* */
		time = timeIt(searchTwo, x, v, n, CLOCK_PROCESS_CPUTIME_ID);
		printf("Time for Thir :~ %5llu\n", time);
		/* */
		time = timeIt(searchOriginal, x, v, n, CLOCK_PROCESS_CPUTIME_ID);
		printf("Time for Orig :~ %5llu\n", time);
		/* */
		time = timeIt(searchOne, x, v, n, CLOCK_PROCESS_CPUTIME_ID);
		printf("Time for Seco :~ %5llu\n", time);
		/* */

	}
	return 0;
}

