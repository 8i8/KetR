#include <stdio.h>

int test(char* test1, char* test2)
{
	char *s, *t;
	s = test1;
	t = test2;

	printf("Test 1 -> %s %s\n", s, s+100);
	printf("Test 2 -> %s %s\n", t, t+100);

	return 0;
}

int main(void)
{
	char test1[100][100] = 	{{ 'H','e','l','l','o' },
				 { 'W','o','r','l','d' }};
	char test2[100][100];

	test2[0][0]= 'H';
	test2[0][1]= 'e';
	test2[0][2]= 'l';
	test2[0][3]= 'l';
	test2[0][4]= 'o';
                   
	test2[1][0]= 'W';
	test2[1][1]= 'o';
	test2[1][2]= 'r';
	test2[1][3]= 'l';
	test2[1][4]= 'd';

	char* s;
	char* t;

	s = (char*)test1;
	t = (char*)test2;

	test(s, t);
	
	return 0;
}

