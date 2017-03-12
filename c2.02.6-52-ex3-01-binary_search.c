/*
 * Exercise 3-1. Our binary search makes two tests inside the loop, when one
 * would suffice (at the price of more tests outside.) Write a version with
 * only one test inside the loop and measure the difference in run- time.
 *
 * This program generates a random number between the defined limits and then
 * an array with that amount of cells. The array is then filled with random
 * values and sorted into ascending order; Passed through three different
 * search functions, each of which is timed.
 */
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define DEBUG		0
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
	printf("The value is not in the list.\n");
	return 0;
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
	printf("The value is not in the list.\n");
	return 0;
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
	else
	{
		/* no match */
		printf("The value is not in the list.\n");
		return 0;
	}
}

int noSearch(int x, int v[], int n)
{
	return -1;
}

/*
 * Return a random value between the given limits.
 */
int myRand(int min, int max)
{
	int range;
	int x;

	if(min > max) {
		x = min;
		min = max;
		max = x;
	}

	range = max - min;
	srand(time(NULL));
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

int timeval_subtract (struct timeval *result, struct timeval *x, struct timeval *y)
{
	/* Perform the carry for the later subtraction by updating y. */
	if (x->tv_usec < y->tv_usec) {
		int nsec = (y->tv_usec - x->tv_usec) / 1000000 + 1;
		y->tv_usec -= 1000000 * nsec;
		y->tv_sec += nsec;
	}
	if (x->tv_usec - y->tv_usec > 1000000) {
		int nsec = (x->tv_usec - y->tv_usec) / 1000000;
		y->tv_usec += 1000000 * nsec;
		y->tv_sec -= nsec;
	}

	/* Compute the time remaining to wait.
		 tv_usec is certainly positive. */
	result->tv_sec = x->tv_sec - y->tv_sec;
	result->tv_usec = x->tv_usec - y->tv_usec;

	/* Return 1 if result is negative. */
	return x->tv_sec < y->tv_sec;
}

/*
 * Run the provided search function whilst timing the operation.
 */
void timeIt(char* text, search_fn fn, int x, int v[], int n)
{
	int output;
	clock_t begin = clock();
	output = (*fn)(x, v, n);
	clock_t end = clock();
	double time = (double)(end - begin) / CLOCKS_PER_SEC;
	if(DEBUG)
		printf("x --> %9d n --> %9d v[n] --> %9d\n", x, n, v[n-1]);
	if(output)
		printf("%s \t: %9d\t\t ~ Time : %f\n", text, output, time);
}

int main(void)
{
	int c;

	puts("Hit a key to start ...");

	while((c = getchar()) != 'q')
	{
		int n = myRand(A_MIN, A_LENGTH);
		int x = myRand(A_MIN, A_LENGTH);
		int v[n];

		fillArray(v, n);
		sortArray(v, n);

		search_fn searchOriginal;
		search_fn searchOne;
		search_fn searchTwo;

		searchOriginal = binsearch;
		searchOne = binarySearchOne;
		searchTwo = binarySearchTwo;

		timeIt("No search", noSearch, x, v, n);
		/* */
		timeIt("Search Orig", searchOriginal, x, v, n);
		/* */
		timeIt("Search One", searchOne, x, v, n);
		/* */
		timeIt("Search Two", searchTwo, x, v, n);
	}
	return 0;
}

