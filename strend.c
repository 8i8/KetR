/*
 * Exercise 5-4. Write the function strend(s,t), which returns 1 if the string
 * t occurs at the end of the string s, and zero otherwise.
 */
#include <stdio.h>

/*
 * A bug in here ... ?
 */
int strend(char *s, char *t)
{
	size_t i, j = 0;

	while (*s)
		i++, s++;
	while (*t)
		j++, t++;

	if(i > j) {
		while (j > 0) {
			if (*--s == *--t) {
				--j, --i;
			} else {
				--j, --i;
				return 0;
			}
		}
		return 1;
	}
	return 0;
}

int main(void)
{
	char s[12] = { 'H','e','l','l','o',' ','W','o','r','l','d','\0' };
	char t[6] = { 'w','o','r','l','d','\0' };
	//char s[] = { "Hello World" };
	//char t[] = { "World" };

	if (strend(s, t))
		puts("Yes");
	else
		puts("No");
	return 0;
}

