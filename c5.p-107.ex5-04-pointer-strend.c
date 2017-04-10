/*
 * Exercise 5-4. Write the function strend(s,t), which returns 1 if the string
 * t occurs at the end of the string s, and zero otherwise.
 */
#include <stdio.h>

/*
 * strend using pointer arithmetic, with moving iterated pointers.
 */
static int strend(char *s, char *t)
{
	size_t i, j = 0;
	char a, b;

	while (*s)
		i++, *s++;
	while (*t)
		j++, *t++;

	if(i > j) {
		while (j > 0) {
			if (*--s == *--t) {
				--j, --i;
			} else
				return 0;
		}
		return 1;
	}
	return 0;
}

/*
 * strend using pointer arithmetic, stationary pointers.
 */
static int _strend(char *s, char *t)
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
	char t[6] = { 'W','o','r','l','d','\0' };

	if (strend(s, t))
		puts("Yes");
	else
		puts("No");

	return 0;
}

