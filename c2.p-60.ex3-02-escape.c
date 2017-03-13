/*
 * Exercise 3-2. Write a function escape(s,t) that converts characters like
 * newline and tab into visible escape sequences like \n and \t as it copies
 * the string t to s. Use a switch. Write a function for the other direction as
 * well, converting escape sequences into the real characters.
 */
#include <stdio.h>

#define MAXLEN	1000

void pString(char pre[], char string[]);
void escape(char s[], char t[]);
void retunTo(char t[], char s[]);
void inputToLine(char input[], char s[]);
int readSpecialChar(char s[]);
int getInput(char input[], int lim);

int main(void)
{
	char input[MAXLEN+1];
	int len;
	int count;

	len = getInput(input, MAXLEN);
	pString("input", input);
	/* */
	char s[len];
	inputToLine(input, s);
	pString("s",s);
	/* */
	count = readSpecialChar(s);
	char t[len + count];
	escape(s, t);
	pString("t",t);
	/* */
	char s2[len];
	retunTo(t, s2);
	pString("s2",s2);

	return 0;
}

/*
 * Print string to terminal.
 */
void pString(char pre[], char string[])
{
	printf("%s\t-->\t%s\n", pre, string);
}

/*
 * Replace escaped characters with their writen characters.
 */
void escape(char s[], char t[])
{
	int i;
	int j;
	i = j = 0;

	while(s[i])
	{
		switch(s[i])
		{
			case '\n' : t[j++] = '\\'; t[j++] = 'n';
				    break;
			case '\t' : t[j++] = '\\'; t[j++] = 't';
				    break;
			case '\v' : t[j++] = '\\'; t[j++] = 'v';
				    break;
			case '\b' : t[j++] = '\\'; t[j++] = 'b';
				    break;
			case '\r' : t[j++] = '\\'; t[j++] = 'r';
				    break;
			case '\f' : t[j++] = '\\'; t[j++] = 'f';
				    break;
			case '\a' : t[j++] = '\\'; t[j++] = 'a';
				    break;
			case '\\' : t[j++] = '\\'; t[j++] = '\\';
				    break;
			case '\?' : t[j++] = '\\'; t[j++] = '?';
				    break;
			case '\'' : t[j++] = '\\'; t[j++] = '\'';
				    break;
			case '\"' : t[j++] = '\\'; t[j++] = '\"';
				    break;
			default   : t[j++] = s[i];
				  break;
		}
		i++;
	}
	t[j++] = '\0';
}

/*
 * Change textual representation of ecascaped characters into correctly escaped
 * characters.
 */
void retunTo(char t[], char s[])
{
	int i;
	int j;
	i = j = 0;

	while(t[i])
	{
		if(t[i] == '\\')
		{
			switch(t[i+1])
			{
				case 'n'  : s[j++] = '\n'; i++;
					     break;
				case 't'  : s[j++] = '\t'; i++;
					     break;
				case 'v'  : s[j++] = '\v'; i++;
					     break;
				case 'b'  : s[j++] = '\b'; i++;
					     break;
				case 'r'  : s[j++] = '\r'; i++;
					     break;
				case 'f'  : s[j++] = '\f'; i++;
					     break;
				case 'a'  : s[j++] = '\a'; i++;
					     break;
				case '\\' : s[j++] = '\\'; i++;
					     break;
				case '?'  : s[j++] = '\?'; i++;
					     break;
				case '\'' : s[j++] = '\''; i++;
					     break;
				case '\"' : s[j++] = '\"'; i++;
					     break;
				default   : s[j++] = t[i++];
					     break;
			}
			i++;
		}
		else
			s[j++] = t[i++];
	}
	t[j++] = '\0';
}

/*
 * Copy the input string into a string of the correct size.
 */
void inputToLine(char input[], char s[])
{
	int i;
	i = 0;

	while (input[i]) {
		s[i] = input[i];
		i++;
	}
	s[i++] = '\0';
}

/*
 * Count the special characters in a string, used to prolong the array for the
 * string copy.
 */
int readSpecialChar(char s[])
{
	int i;
	int count;
	i = count = 0;

	while(s[i])
	{
		switch(s[i])
		{
			case '\n' : case '\b' : case '\t' : case '\\' : case '\v' :
				count++;
				break;
			default : break;
		}
		i++;
	}
	return count;
}

/*
 * Get a text input from the user.
 */
int getInput(char input[], int lim)
{
	int i;
	int c;

	for(i = 0; i < lim-2 && ((c = getchar()) != '\n'); i++)
		input[i] = c;

	if(c == '\n')
		input[i++] = c;

	input[i++] = '\0';

	return i;
}

