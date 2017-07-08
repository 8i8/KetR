/*
 *  Exercise 6-6. Implement a simple version of the #define processor (i.e., no
 *  arguments) suitable for use with C programs, based on the routines of this
 *  section. You may also find getch and ungetch helpful.
 */
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define MAX_WORD	100

typedef short int bool;
enum boolean { false, true };
enum pre { HASH = 2, STATMENT = 3 , NAME = 4, VALUE = 5 };

typedef struct {
	bool strlit;
	bool charlit;
	bool comment;
	bool prepros;
	bool skip;
	bool read;
} status;

struct nlist {
	struct nlist *next;
	char *name;
	char *defn;
};

static size_t getword(char* word, size_t lim, status *state);
static int undef(char *def);
static struct nlist *lookup(char *s);
static struct nlist *install(char *name, char*defn);

int main(void)
{
	struct nlist *test;
	char word[MAX_WORD];
	status state;

	install("define", "one");

	while ((getword(word, MAX_WORD, &state)) != EOF)
	{
		printf("%s", word);
	}

	if ((test = lookup("MAX_WORD")) != NULL)
		printf("Test result -> %s %s\n", test->name, test->defn);
	else
		printf("Not yet!\n");

	undef("define");

	return 0;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  getword
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
static int getch(void);
static void ungetch(int c);

/*
 * gettoken:	read untill a space or nonalpha numeric character is read
 *		allow '_'
 */
static char* gettoken(char *word, char *w, size_t lim, status *state)
{
	for ( ; --lim > 0; w++)
		if (!isalnum(*w = getch()) && *w != '_') {
			ungetch(*w);
			*w = '\0';
			break;
		}

	return w;
}

/*
 * readtoken:	read and compaire tokens
 */
static char* readtoken(char *word, char *w, size_t lim, status *state)
{
	static char store[MAX_WORD];
	struct nlist *s;

	if ((s = lookup(word)) != NULL) {
		if (!strcmp(s->name, "define"))
			state->read = STATMENT;
		else
			strcpy(word, s->defn);
	} else if (state->read == STATMENT) {
		strcpy(store, word);
		state->read = NAME;
	} else if (state->read == NAME) {
		install(store, word);
		state->read = true;
	}

	return w;
}

static size_t getword(char* word, size_t lim, status *state)
{
	char c;
	char *w = word;

	if ((c = getch()) == '\n' && state->prepros && !state->skip)
		state->prepros = false;
	else if (c == '\n' && state->prepros && state->skip)
		state->skip = false;

	if (c != EOF)
		*w++ = c;

	if (c == '\'') {
		if (!state->charlit)
		       state->charlit = true;
		else
			state->charlit = false;
	} else if (c == '"') {
		if (!state->strlit && !state->charlit)
			state->strlit = true;
		else
			state->strlit = false;
	} else if (c == '/' && !state->comment) {
		if ((c = getch()) == '*') {
			state->comment = true;
			*--w = '\0';
			return *w;
		} else
			ungetch(c);
	} else if (c == '*' && state->comment) {
		if ((c = getch()) == '/') {
			state->comment = false;
			while (isspace(getch()))
				;
			*--w = '\0';
			return *w;
		} else {
			*--w = '\0';
			return *w;
		}
	} else if (c == '#' && (!state->comment && !state->strlit && !state->prepros)) {
		state->prepros = HASH;
	} else if (c == '\\' && state->prepros)
		state->skip = true;

	if (state->comment) {
		*--w = '\0';
		return c;
	}

	if (state->strlit || (!isalnum(c) && c != '_')) {
		*w = '\0';
		return c;
	}
	
	if (state->prepros == HASH) {
		state->prepros = STATMENT;
	} else if (state->prepros == STATMENT) {
		state->prepros = NAME;
	} else if (state->prepros == NAME) {
		state->prepros = VALUE;
	}

	w = gettoken(word, w, lim, state);
	w = readtoken(word, w, lim, state);

	return *w;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  getch
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
#define BUFSIZE	100
static char buf[BUFSIZE];
static size_t bufp = 0;

/*
 * getch:	Get the next char, from the buffer if full, else by getchar()
 */
static int getch(void)
{
	return (bufp > 0) ? buf[--bufp] : (char)getchar();
}

/*
 * ungetch:	Push back the most recent char into the buffer
 */
static void ungetch(int c)
{
	if (bufp >= BUFSIZE)
		printf("ungetch: Buffer full");
	else
		buf[bufp++] = c;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Hash table
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
#define HASHSIZE	101
static struct nlist *hashtab[HASHSIZE];
/*
 * hash:	form hash value for string s
 */
static unsigned hash(char *s)
{
	unsigned hashval;

	for (hashval = 0; *s != '\0'; s++)
		hashval = *s + 31 * hashval;
	return hashval % HASHSIZE;
}

/*
 * undef:	free up and remove entry
 */
static int undef(char *def)
{
	struct nlist *np;

	np = lookup(def);

	if (np != NULL) {
		free(np->name);
		free(np->defn);
		free(hashtab[hash(def)]);
		return 1;
	} else
		return 0;
}

/*
 * lookup:	Look for s in hashtab
 */
static struct nlist *lookup(char *s)
{
	struct nlist *np;

	for (np = hashtab[hash(s)]; np != NULL; np = np->next)
		if (strcmp(s, np->name) == 0)
			return np;
	return NULL;
}

/*
 * install:	put (name, defn) in hashtab
 */
static struct nlist *install(char *name, char *defn)
{
	struct nlist *np;
	unsigned hashval;

	if ((np = lookup(name)) == NULL) {
		np = (struct nlist *) malloc(sizeof(*np));
		if (np == NULL || (np->name = strdup(name)) == NULL)
			return NULL;
		hashval = hash(name);
		np->next = hashtab[hashval];
		hashtab[hashval] = np;
	} else
		free((void *) np->defn);
	if ((np->defn = strdup(defn)) == NULL)
		return NULL;
	return np;
}

