/*
 * Exercise 5-5. Write versions of the library functions strncpy, strncat, and
 * strncmp, which operate on at most the first n characters of their argument
 * strings. For example, strncpy(s,t,n) copies at most n characters of t to s.
 * Full descriptions are in Appendix B.
 *
 * p.107
 */
#include <stdio.h>

/*
 * Copy the first n characters from string ct to string s.
 */
char *_strncpy(char *s, char *ct, size_t n)
{
	size_t i = 0;

	while ((*(s+i) = *(ct+i)) && --n > 0)
		i++;

	*(s+(++i)) = '\0';

	return s;
}

char *__strncpy(char *s_in, char *ct_in, size_t n)
{
	size_t i = 0;
	char *s, *ct;
	s = s_in;
	ct = ct_in;

	while ((*s++ = *ct++) && --n > 0)
		i++;

	*++s = '\0';
	s = s_in;

	return s;
}

/*
 * Concatenate the first n characters from string t onto the end of string s.
 */
char *_strncat(char *s, char *ct, size_t n)
{
	char *s_in;
	s_in = s;

	while (*s)
		*s++;

	while (n-- > 0)
		*s++ = *ct++;

	*++s = '\0';
	s = s_in;

	return s;
}

/*
 * Compaire the first n characters of string s with n characters from string t.
 */
int _strncmp(char *cs, char *ct, size_t n)
{
	while ((*cs++ == *ct++) && --n > 0)
		;
	if (n == 0)
		return 1;
	return 0;
}

int main(void)
{
	char ct[] = "Hello World";
	char s[12];
	char s2[12] = "Hello ";
	char ct2[6] = "World";
	char cs3[] = "Hello World";
	char ct3[] = "Hello Bob";
	char *t1, *t2;

	t1 = __strncpy(s, ct, 5);
	printf("1 --> %s\n", t1);

	t2 = _strncat(s2, ct2, 5);
	printf("2 --> %s\n", t2);

	if(_strncmp(cs3, ct3, 8))
		printf("Yes\n");
	else
		printf("No\n");

	return 0;
}

