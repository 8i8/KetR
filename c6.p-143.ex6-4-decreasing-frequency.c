/*
 * Exercise 6-4. Write a program that prints the distinct words in its input
 * sorted into decreasing order of frequency of occurrence. Precede each word
 * by its count.
 */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAXWORD	100
#define SKIP	(sizeof(skiplist)/sizeof(char *))

typedef short int bool;
enum boolean { false, true };
enum counter { VALUE, COUNT };
enum searchb { UPPER, LOWER };

struct nnode {
	int *linenum;
	size_t count;
	struct nnode *left;
	struct nnode *right;
};

struct tnode {
	char *word;
	size_t count;
	struct nnode *ln;
	struct tnode *left;
	struct tnode *right;
};

static char *skiplist[] = {
	"a", "an", "and", "are", "be", "can", "has", "have", "he", "i", "if", "in",
	"is", "it", "of", "so", "that", "the", "then", "this", "to", "which", "will"
};

static int compcount (const void *a, const void *b)
{
	struct tnode *tnodeA = *(struct tnode **)a;
	struct tnode *tnodeB = *(struct tnode **)b;
	return tnodeA->count - tnodeB->count;
}

static int compword (const void *a, const void *b)
{
	struct tnode *tnodeA = *(struct tnode **)a;
	struct tnode *tnodeB = *(struct tnode **)b;
	return strcmp(tnodeB->word, tnodeA->word);
}

static size_t getword(char* word, size_t lim);
static struct tnode *addtree(struct tnode *p, char *w, int ln);
static int checklist(char* w, char* list[], size_t len);
static int nodeCount(bool aug);
static void treearray(struct tnode *p, struct tnode *array[]);
static void sortarray(struct tnode *p[], size_t len);
static void printarray(struct tnode *p[], size_t len);

int main(void)
{
	long line = 0;
	struct tnode *root;
	char word[MAXWORD];

	root = NULL;
	while ((line = getword(word, MAXWORD)) != EOF)
		if (isalpha(word[0]) && !checklist(word, skiplist, SKIP))
			root = addtree(root, word, line);

	size_t len;
	len = nodeCount(VALUE);
	struct tnode *tnodearray[len];

	treearray(root, tnodearray);
	sortarray(tnodearray, len);
	printarray(tnodearray, len);

	return 0;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Check list
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
/*
 * checklist:	check against list of words to ignore
 */
static int checklist(char* w, char* list[], size_t len)
{
	size_t i, j;
	j = strlen(w);
	char t[j];

	for (i = 0; i < j; i++)
		t[i] = tolower(w[i]);
	t[i] = '\0';

	for (i = 0; i < len; i++)
		if (strcmp(list[i], t) == 0)
			return 1;
	return 0;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Tree and nodes
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
static struct nnode *addnum(struct nnode *n, int ln);
static struct tnode *talloc(void);
static struct nnode *nalloc(void);
static char *_strdup(char *);
static int *_arydup(int *a, int ln, int len);

/*
 * addtree:	Add a node with w, at or below p
 */
static struct tnode *addtree(struct tnode *p, char *w, int ln)
{
	struct nnode *root;
	int cond;

	root = NULL;
	if (p == NULL) {	/* New word arrived, (word not found) */
		p = talloc();	/* Make node */
		p->word = _strdup(w);
		p->count = 1;
		p->left = p->right = NULL;
		p->ln = addnum(root, ln);
		nodeCount(COUNT);
	} else if ((cond = strcmp(w, p->word)) == 0) {
		p->count++;	/* repeated word */
		p->ln->linenum = _arydup(p->ln->linenum, ln, p->count);
	} else if (cond < 0)	/* less than, into left subtree */
		p->left = addtree(p->left, w, ln);
	else			/* greater than, into tree right */
		p->right = addtree(p->right, w, ln);
	return p;
}

/*
 * addnum:	Add a new line number ln, at or below n
 */
static struct nnode *addnum(struct nnode *n, int ln)
{
	int *root;
	root = NULL;
	if (n == NULL) {
		n = nalloc();
		n->count = 1;
		n->linenum = _arydup(root, ln, n->count);
		n->left = n->right = NULL;
	} else if (ln == n->linenum[n->count-1]) {
		n->count++;
		n->linenum = _arydup(n->linenum, ln, n->count);
	} else if (ln > n->linenum[n->count-1])
		n->left = addnum(n->left, ln);
	else
		n->right = addnum(n->right, ln);
	return n;
}

/*
 * talloc:	make a tnode
 */
static struct tnode *talloc(void)
{
	return (struct tnode *) malloc(sizeof(struct tnode));
}

/*
 * nalloc:	make a nnode
 */
static struct nnode *nalloc(void)
{
	return (struct nnode *) malloc(sizeof(struct nnode));
}

/*
 * makeary:	make an array
 */
static int *makeary(int len)
{
	return (int *) malloc(sizeof(int)*len);
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

/*
 * _arydup:	extend array by one and fill
 */
static int *_arydup(int *a, int ln, int len)
{
	if (a == NULL) {
		a = makeary(len-1);
		*a = ln;
		return a;
	}

	int i, *p;
	p = NULL;

	p = makeary(len);

	for (i = 0; i < len-1; i++)
		p[i] = a[i];

	free(a);
	p[i] = ln;

	return p;
}

/*
 * nodeCount:	Keep count of how many tnodes are made
 */
static int nodeCount(bool aug)
{
	static size_t count = 0;

	if (aug)
		return ++count;
	else
		return count;
}

/*
 * treearray:	Fill an array of structs for sorting
 */
static void treearray(struct tnode *p, struct tnode *array[])
{
	static size_t count = 0;

	if (p != NULL) {
		treearray(p->left, array);
		array[count++] = p;
		treearray(p->right, array);
	}
}

/*
 * binarysearch:	Returns the index of the upper and lower instances of a
 * 			number in an array
 */
static int binarysearch(struct tnode *p[], size_t len, size_t x, bool searchFirst)
{
	ssize_t low = 0, high = len-1, result = -1;

	while (low <= high)
	{
		ssize_t mid = low+((high-low)/2);
		if (p[mid]->count == x)
		{
			result = mid;
			if (searchFirst)
				high = mid-1;
			else
				low = mid+1;

		}
		else if (x < p[mid]->count)
			high = mid-1;
		else low = mid+1;
	}
	return result;
}

/*
 * sortarray:	Sort the array by word frequency descending and then by word
 * 		alphabeticaly assending
 */
static void sortarray(struct tnode *p[], size_t len)
{
	qsort(p, len, sizeof(struct tnode *), compcount);

	size_t lower, upper = 0;

	do {
		lower = upper;
		lower = binarysearch(p, len, p[lower]->count, LOWER);
		upper = binarysearch(p, len, p[lower]->count, UPPER);
		upper++;
		qsort(p+lower, upper-lower, sizeof(struct tnode *), compword);
	} while(p[upper]->count < len);
}

/*
 * printarray:	Print array to terminal
 */
static void printarray(struct tnode *p[], size_t len)
{
	while (len--)
		printf("%4lu: %s\n", p[len]->count, p[len]->word);
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  input ~ getword
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
static int getch(void);
static void ungetch(int c);

static size_t getword(char* word, size_t lim)
{
	static size_t line = 1;
	int c;
	char *w = word;

	while (isspace(c = getch()))
		if (c == '\n')
			++line;

	if (c != EOF)
		*w++ = c;

	if (!isalpha(c)) {
		*w = '\0';
		return c;
	}

	for ( ; --lim > 0; w++)
		if(!isalnum(*w = getch())) {
			ungetch(*w);
			break;
		}

	*w = '\0';
	return line;

}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  getch buffer
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
#define BUFSIZE 100
static char buf[BUFSIZE];
static int p_buf = 0;

/*
 * getch:	get the next char
 */
static int getch(void)
{
	return (p_buf > 0) ? buf[--p_buf] : getchar();
}

/*
 * ungetch:	Get a possible pushed back character
 */
static void ungetch(int c)
{
	if (p_buf >= BUFSIZE)
		printf("ungetch: too many characters\n");
	else
		buf[p_buf++] = c;
}

