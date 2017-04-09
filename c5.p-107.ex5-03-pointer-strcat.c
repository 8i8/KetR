/*
 * Exercise 5-3. Write a pointer version of the function strcat that we showed
 * in Chapter 2: strcat(s,t) copies the string t to the end of s.
 *
 * p.107
 */
#include <stdio.h>

/*
 * strcat using arrays.
 */
void __strcat(char s[], char t[])
{
	size_t i, j;

	i = j = 0;
	while (s[i] != '\0')
		i++;
	while ((s[i++] = t[j++]) != '\0')
		;
}

/*
 * strcat using pointers.
 */
void _strcat(char *s, char *t)
{
	while (*s)
		*s++;
	while (*s++ = *t++)
		;
}

int main(void)
{
	char s[12] = { 'H','e','l','l','o',' ','\0' };
	char t[6] = { 'W','o','r','l','d','\0' };

	_strcat(s, t);
	printf("%s\n", s);
	return 0;
}

