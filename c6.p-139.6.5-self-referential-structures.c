/*
 * Count the number of occurance of all words in a document.
 */
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define MAXWORD	100

/*
 * The tree node.
 */
struct tnode {
	char *word;		/* The text */
	int count;		/* Number of occurrences */
	struct tnode *left;	/* Left child */
	struct tnode *right;	/* Right child */
};

static struct tnode *addtree(struct tnode *, char *);
static void treeprint(struct tnode *);
static int getword(char *word, int lim);

/*
 * Word frequency count.
 */
int main(void)
{
	struct tnode *root;
	char word[MAXWORD];

	root = NULL;
	while (getword(word, MAXWORD) != EOF)
		if (isalpha(word[0]))
			root = addtree(root, word);

	treeprint(root);
	return 0;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Tree and nodes
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
static struct tnode *talloc(void);
static char *_strdup(char *);

/*
 * addtree:	Add a node with w, at or below p.
 */
static struct tnode *addtree(struct tnode *p, char *w)
{
	int cond;

	if (p == NULL) {	/* new word arrived */
		p = talloc();	/* make a new node */
		p->word = _strdup(w);
		p->count = 1;
		p->left = p->right = NULL;
	} else if ((cond = strcmp(w, p->word)) == 0)
		p->count++;	/* repeated word */
	else if (cond < 0)	/* less than, into left subtree */
		p->left = addtree(p->left, w);
	else			/* greater than, into right subtree */
		p->right = addtree(p->right, w);
	return p;
}

/*
 * treeprint:	in-order, print of tree p
 */
static void treeprint(struct tnode *p)
{
	if (p != NULL) {
		treeprint(p->left);
		printf("%4d %s\n", p->count, p->word);
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

	p = (char *) malloc(strlen(s)+1);	/* +1 for '\0' */
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
 * getword:	get next word or character from input
 */
static int getword(char *word, int lim)
{
	int c;
	char *w = word;

	while (isspace(c = getch()))
		;

	if (c != EOF)
		*w++ = c;

	if (!isalpha(c)) {
		*w = '\0';
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
 * ungetch:	Get a (possibly pushed back) character
 */
static void ungetch(int c)
{
	if (bufp >= BUFSIZE)
		printf("ungetch: too many characters\n");
	else
		buf[bufp++] = c;
}

