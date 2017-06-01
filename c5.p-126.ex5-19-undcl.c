/*
 * Exercise 5-19. Modify undcl so that it does not add redundant parentheses to
 * declarations.
 */
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAXTOKEN	100

enum { NAME, PARENS, BRACKETS };

static int  gettoken(void);
static int  getch(void);
static void ungetch(int);

static int  tokentype;			/* type of last token */
static char token[MAXTOKEN];		/* last token string */
static char out[1000];

/*
 * undcl: convert word descriptions to declarations
 */
int main(void)
{
	int type;
	char temp[MAXTOKEN];

	while (gettoken() != EOF) {
		strcpy(out, token);
		while ((type = gettoken()) != '\n')
			if (type == PARENS || type == BRACKETS)
				strcat(out, token);
			else if (type == '*') {
				sprintf(temp, "(*%s)", out);
				strcpy(out, temp);
			} else if (type == NAME) {
				sprintf(temp, "%s %s", token, out);
				strcpy(out, temp);
			} else
				printf("invalid input at %s\n", token);
		printf("%s\n", out);
	}
	return 0;
}

/*
 * Return next token.
 */
static int gettoken(void) /* return next token */
{
	int c, getch(void);
	void ungetch(int);
	char *p = token;

	while ((c = getch()) == ' ' || c == '\t')
		;
	if (c == '(') {
		if ((c = getch()) == ')') {
			strcpy(token, "()");
				return tokentype = PARENS;
		} else {
			ungetch(c);
			return tokentype = '(';
		}
	} else if (c == '[') {
		for (*p++ = c; (*p++ = getch()) != ']'; )
			;
		*p = '\0';
		return tokentype = BRACKETS;
	} else if (isalpha(c)) {
		for (*p++ = c; isalnum(c = getch()); )
			*p++ = c;
		*p = '\0';
		ungetch(c);
		return tokentype = NAME;
	} else
		return tokentype = c;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Buffer.
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
#define BUFSIZE	100

static char buf[BUFSIZE];	/* Buffer for next ungetch */
static int bufp = 0;		/* next free position in buf */

static int getch(void)		/* get a (possibly pushed back) character */
{
	return (bufp > 0) ? buf[--bufp] : getchar();
}

static void ungetch(int c)	/* push character back on input */
{
	if (bufp >= BUFSIZE)
		printf("ungetch: too many characters\n");
	else
		buf[bufp++] = c;
}

