/*
 * Exercise 5-18. Make dcl recover from input errors.
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAXTOKEN	100

enum { NAME, PARENS, BRACKETS };

static void dcl(void);
static void dirdcl(void);
static int gettoken(void);
static int getch(void);
static void ungetch(int);

static int tokentype;			/* type of last token */
static char token[MAXTOKEN];		/* last token string */
static char name[MAXTOKEN];		/* identifier name */
static char datatype[MAXTOKEN];		/* data type = char, int, etc. */
static char out[1000];

/*
 * Convert declaration to words.
 */
int main(void)
{
	while (gettoken() != EOF) {		/* 1st token on line */
		strcpy(datatype, token); 	/* is the datatype */
		out[0] = '\0';
		dcl();				/* parse rest of line */
		if (tokentype != '\n')
			printf("syntax error\n");
		printf("%s: %s %s\n", name, out, datatype);
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

/*
 * dcl: parse a declarator.
 */
static void dcl(void)
{
	int ns;

	for (ns = 0; gettoken() == '*'; ) /* count *'s */
		ns++;

	dirdcl();

	while (ns-- > 0)
	strcat(out, " pointer to");
}

/*
 * dirdcl: parse a direct declarator.
 */
static void dirdcl(void)
{
	int type;

	/* ( dcl ) */
	if (tokentype == '(') {
		dcl();
		if (tokentype != ')')
			printf("error: missing )\n");
	} else if (tokentype == NAME) /* variable name */
		strcpy(name, token);
	else
		printf("error: expected name or (dcl)\n");
	while ((type=gettoken()) == PARENS || type == BRACKETS)
		if (type == PARENS)
			strcat(out, " function returning");
		else {
			strcat(out, " array");
			strcat(out, token);
			strcat(out, " of");
		}
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

