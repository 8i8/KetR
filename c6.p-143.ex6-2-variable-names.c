/*
 * Exercise 6-2. Write a program that reads a C program and prints in
 * alphabetical order each group of variable names that are identical in the
 * first 6 characters, but different somewhere thereafter. Don't count words
 * within strings and comments. Make 6 a parameter that can be set from the
 * command line.
 *
 * TODO This question is not yet completed.
 */
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

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

/*
 * The tree node.
 */
struct tnode {
	char *word;		/* The text */
	int count;		/* Number of occurrences */
	struct key *vartype;	/* Variable type */
	struct tnode *left;	/* Left child */
	struct tnode *right;	/* Right child */
};

static struct key {
	char *word;
	int count;
} keytab[] = {
	{ "char",	0, },
	{ "bool",	0, },
	{ "double",	0, },
	{ "enum",	0, },
	{ "float",	0, },
	{ "int",	0, },
	{ "struct",	0, }
};

static struct key *binsearch(char *word, struct key *tab, int nkeys);
static struct tnode *addtree(struct tnode *p, struct key *t, char *w);
static void treeprint(struct tnode *p);
static int getword(char *word, int lim, struct status *state);

int main(int argc, char* argv[])
{
	short int wlen = 6;

	if (--argc > 0) {
		if (isdigit(wlen = atof(*++argv)))
			;
		else
			printf("error usage: a.out <number>\n");
	}

	struct tnode *root;
	struct key *p_key;
	struct status state = { false };
	char word[MAXWORD];

	root = NULL;
	while (getword(word, MAXWORD, &state) != EOF) {
		if (isalpha(word[0])) {
			if ((p_key = binsearch(word, keytab, NKEYS)) != NULL) {
				p_key->count++;
				getword(word, MAXWORD, &state);
				while (!isalpha(word[0]) && word[0] != '_')
					getword(word, MAXWORD, &state);
				root = addtree(root, p_key, word);
			}
		}
	}

	treeprint(root);

	return 0;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Search
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
static struct key *binsearch(char *word, struct key *tab, int nkeys)
{
	int cond;
	struct key *low = &tab[0];
	struct key *high = &tab[nkeys];
	struct key *mid;

	while (low < high) {
		mid = low+(high-low) / 2;
		if ((cond = strcmp(word, mid->word)) < 0)
			high = mid;
		else if (cond > 0)
			low = mid + 1;
		else
			return mid;
	}
	return NULL;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Tree and nodes
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
static struct tnode *talloc(void);
static char *_strdup(char *);

static struct tnode *addtree(struct tnode *p, struct key *t, char *w)
{
	int cond;

	if (p == NULL) {
		p = talloc();
		p->word = _strdup(w);
		p->count = 1;
		p->left = p->right = NULL;
	} else if ((cond = strcmp(w, p->word)) == 0) {
		p->count++;
		p->vartype = t;
	} else if (cond < 0)
		p->left = addtree(p->left, t, w);
	else
		p->right = addtree(p->right, t, w);
	return p;
}

/*
 * treeprint:	in-order print of tree p
 */
static void treeprint(struct tnode *p)
{
	if (p != NULL) {
		treeprint(p->left);
		if (p->vartype != NULL)
			printf("%s\t%4d %s\n", p->vartype->word, p->count, p->word);
		else
			printf("NULL\t%4d %s\n",p->count, p->word);
		treeprint(p->right);
	}
}

/*
 * talloc:	make a tnode
 */
static struct tnode *talloc(void)
{
	return (struct tnode *) malloc(sizeof(struct tnode));
}

/*
 * _strdup:	make a duplicate of s
 */
static char *_strdup(char *s)
{
	char *p;

	p = (char *) malloc(strlen(s)+1);
	if (p != NULL)
	       strcpy(p, s);
	return p;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  input
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
static int getch(void);
static void ungetch(int c);

/*
 * getword:	get next word of character from input
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
		else if (state->charlit)
			state->charlit = false;
	}

	if (c != EOF)
		*w++ = c;

	if (c == '\'') {
		if (!state->charlit)				/* Set state in string */
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
	} else if (c == '\\' && (state->prepros || state->charlit)) {
		if ((c = getch()) == '\\') {
			;
		} else {
			ungetch(c);
			state->skip = true;
		}
	}

	if (state->strlit || state->charlit || state->comment || state->prepros || (!isalpha(c) && c != '_')) {
		*--w = '\0';
		return c;
	}

	for ( ; --lim > 0; w++)
		if (!isalnum(*w = getch())) {
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
#define BUFSIZE 100

static char buf[BUFSIZE];
static size_t bufp = 0;

/*
 * getch:	get the next char
 */
static int getch(void)
{
	return (bufp > 0) ? buf[--bufp] : getchar();
}

/*
 * ungetch:	Get a (possibly pushed back) character
 */
static void ungetch(int c)
{
	if (bufp >= BUFSIZE)
		printf("ungetch: too many characters\n");
	else
		buf[bufp++] = c;
}

