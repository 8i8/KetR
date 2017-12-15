/*
 * Exercise 6-1. Our version of getword does not properly handle underscores,
 * string constants, comments, or preprocessor control lines. Write a better
 * version.
 */
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAXWORD	100
#define NKEYS	(sizeof keytab / sizeof(struct key))

typedef short int bool;
enum boolean { false, true };

struct status {
	bool strlit;
	bool charlit;
	bool comment;
	bool prepros;
	bool skip;
};

static struct key {
	char *word;
	int count;
} keytab[] = {
	{ "auto",	0, },
	{ "break",	0, },
	{ "case",	0, },
	{ "char",	0, },
	{ "const",	0, },
	{ "continue",	0, },
	{ "default",	0, },
	{ "return",	0, },
	{ "unsigned",	0, },
	{ "void",	0, },
	{ "volatile",	0, },
	{ "while",	0, }
};

static struct key *binsearch(char *, struct key tab[], int);
static int getword(char *word, int lim, struct status *state);

/*
 * Count C keywords, pointer version.
 */
int main(void)
{
	char word[MAXWORD];
	struct key *p;
	struct status state = { false };

	while (getword(word, MAXWORD, &state) != EOF)
		if (isalpha(word[0]))
			if ((p = binsearch(word, keytab, NKEYS)) != NULL)
				p->count++;

	for (p = keytab; p < keytab + NKEYS; p++)
		if (p->count > 0)
			printf("%4d %s\n", p->count, p->word);
	return 0;
}

/*
 * binsearch:	find words in tab[0] ... tab[n-1]
 */
static struct key *binsearch(char *word, struct key tab[], int n)
{
	int cond;
	struct key *low = &tab[0];
	struct key *high = &tab[n];
	struct key *mid;

	while (low < high) {
		mid = low + (high-low) / 2;
		if ((cond = strcmp(word, mid->word)) < 0)
			high = mid;
		else if (cond > 0)
			low = mid + 1;
		else
			return mid;
	}
	return NULL;
}

static int getch(void);
static void ungetch(int c);

/*
 * getword:	get next word or character from input. char
 */
static int getword(char *word, int lim, struct status *state)
{
	int c;
	char *w = word;

	while (isspace(c = getch())) {
		/* Escape all keywords during preprocessor commands */
		if (c == '\n' && state->prepros && !state->skip)
			state->prepros = false;
		else if (c == '\n' && state->prepros && state->skip)
			state->skip = false;
	}

	if (c != EOF)
		*w++ = c;

	if (c == '\'') {
		if (!state->strlit)				/* Set state in string */
			state->charlit = true;
		else
			state->charlit = false;
	} else if (c == '"') {
		if (!state->strlit && !state->charlit)		/* Set state in string */
			state->strlit = true;
		else
			state->strlit = false;
	} else if (c == '/' && !state->comment) {
		if ((c = getch()) == '*')			/* Set state in comment */
			state->comment = true;
		else
			ungetch(c);
	} else if (c == '*' && state->comment) {
		if ((c = getch()) == '/')
			state->comment = false;
		else
			ungetch(c);
	} else if (c == '#') {
		if ((c = getch()) == 'd')			/* Set state in preprocessor */
			state->prepros = true;
		else
			ungetch(c);
	} else if (c == '\\' && state->prepros)
		state->skip = true;

	if (
		state->strlit
		|| state->comment
		|| state->prepros
		|| (!isalpha(c) && c != '_'))
	{
		*w = '\0';
		return c;
	}

	for ( ; --lim > 0; w++)
		if (!isalnum(*w = getch()) && *w != '_') {
			ungetch(*w);
			break;
		}
	*w = '\0';
	return word[0];
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  getch buffer
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
#define BUFSIZE	100

static char buf[BUFSIZE];	/* buffer for ungetch */
static size_t bufp = 0;		/* next free position in buf */

/*
 * getch:	get the next char.
 */
static int getch(void)
{
	return (bufp > 0) ? buf[--bufp] : (char)getchar();
}

/*
 * ungetch:	Get a (possibly pushed back) character.
 */
static void ungetch(int c)
{
	if (bufp >= BUFSIZE)
		printf("ungetch: too many characters\n");
	else
		buf[bufp++] = c;
}

