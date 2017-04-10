/*
 * Exercise 5-4. Write the function strend(s,t), which returns 1 if the string
 * t occurs at the end of the string s, and zero otherwise.
 */
#include <stdio.h>

/*
 * Although this version of strend works, it is not at all preferable as there
 * is an increased risk that the programmer leave the pointers in the middle
 * of the string by forgetting to return them to the start of the string.
 */
int strend(char *s, char *t)
{
	size_t i, j = 0;

	while (*s)
		i++, *s++;
	while (*t)
		j++, *t++;

	if(i > j) {
		while (j > 0) {
			if (*--s == *--t) {
				j--, i--;
			} else  {
				*s = *s-i, *t = *t-j;
				return 0;
			}
		}
		*s = *s-i;
		return 1;
	}
	return 0;
}

/*
 * Here the problem of returning the pointer is solved by making copy's of those
 * pointers and using them during the operation in the place of the original
 * pointers received as arguments.
 */
int _strend(char *s, char *t)
{
	size_t i, j = 0;

	char *s1, *t1;

	s1 = s, t1 = t;

	while (*s1)
		i++, *s1++;
	while (*t1)
		j++, *t1++;

	if(i > j) {
		while (j > 0) {
			if (*--s1 == *--t1) {
				j--;
			} else 
				return 0;
		}
		return 1;
	}
	return 0;
}

/*
 * Here the problem is resolved by performing arithmetic on the integer i and j
 * rather than moving the pointers.
 */
int __strend(char *s, char *t)
{
	size_t i, j = 0;

	while (*(s + i) != '\0')
		i++;
	while (*(t + j) != '\0')
		j++;

	if(i > j) {
		while (j > 0) {
			if (*(s + --i) == *(t + --j)) {
				;
			} else 
				return 0;
		}
		return 1;
	}
	return 0;
}


int main(void)
{
	char s[12] = { 'H','e','l','l','o',' ','W','o','r','l','d','\0' };
	char t[6] = { 'w','o','r','l','d','\0' };

	if (_strend(s, t))
		puts("Yes");
	else
		puts("No");
	return 0;
}

