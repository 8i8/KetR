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
#define HASHSIZE	101

typedef short int bool;
enum boolean { false, true, HASH, STATMENT, NAME, VALUE };

/* States for parser operation */
typedef struct {
	bool strlit;
	bool charlit;
	bool comment;
	bool prepros;
	bool skip;
	bool read;
	bool escape;
} status;

/* hash table targts */
struct nlist {
	struct nlist *next;
	char *name;
	char *defn;
};


static int getword(char* word, size_t lim, status *state);
static int undef(char *def);
static struct nlist *lookup(char *s);
static struct nlist *install(char *name, char*defn);
//static void freeall(struct nlist **np, size_t len);
/* */
static struct nlist *hashtab[HASHSIZE];

static void printstate(status *state)
{
//	printf("strlit -> %d, charlit -> %d, comment -> %d, prepros -> %d, skip -> %d, read -> %d.\n",
//		state->strlit, state->charlit, state->comment, state->prepros, state->skip, state->read);
}

int main(void)
{
	char word[MAX_WORD];
	status state;

	state.strlit = state.charlit = state.comment = state.prepros = state.skip = state.read = 0;

	install("define", "one");

	while ((getword(word, MAX_WORD, &state)) != EOF) {
		printf("%s", word);
		if (!strcmp(word, "\n"))
			printstate(&state);
	}

	undef("define");
	//freeall(hashtab, HASHSIZE);

	return 0;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  getword
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
static int getch(void);
static void ungetch(int c);

/*
 * gettoken:	Input, write to the string word, stop at non alpha numeric
 */
static void gettoken(char *w, size_t lim)
{
	for ( ; --lim > 0; w++)
		if (!isalnum(*w = getch()) && *w != '_') {
			ungetch(*w);
			*w = '\0';
			break;
		}
}

/*
 * readtoken:	read and compaire tokens
 */
static void readtoken(char *word, status *state)
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
		state->read = false;
	}
}

/*
 * getword:	Sort the input stream
 */
static int getword(char* word, size_t lim, status *state)
{
	char c;
	char *w = word;

	/*
	 * State of preprosessor, reset on newline or keep the state.
	 */
	if ((c = getch()) == '\n' && state->prepros && !state->skip)
		state->prepros = false;
	else if (c == '\n' && state->prepros && state->skip)
		state->skip = false;

	if (c != EOF)
		*w++ = c;

	/*
	 * States for comments, strings, char, escaped char and #.
	 */
	if (c == '\'') {
		if (!state->charlit && !state->escape)
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
			while (isspace(c = getch()))
				;
			ungetch(c);
			*--w = '\0';
			return *w;
		} else {
			*--w = '\0';
			return *w;
		}
	} else if (c == '#' && (!state->comment && !state->strlit && !state->prepros)) {
		state->prepros = true;
	} else if (c == '\\' && state->prepros)
		state->skip = true;
	else if (c == '\\' && state->charlit)
		state->escape = true;
	else if (state->escape)
		state->escape = false;

	/*
	 * Back out for comments and strings.
	 */
	if (state->comment) {
		*--w = '\0';
		return c;
	} else if (state->strlit || (!isalnum(c) && c != '_')) {
		*w = '\0';
		return c;
	}
	
	/*
	 * Go get a token.
	 */
	gettoken(w, lim);

	/*
	 * Read and treat the token.
	 */
	readtoken(word, state);

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
		free(np->next);
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

/*
 * freeall:	free all memory from hashtab
 */
//static void freeall(struct nlist **nl, size_t len)
//{
//	size_t i, j;
//	struct nlist *np;
//	struct nlist *last[HASHSIZE];
//
//	for (i = 0; i < len; i++) {
//		if(nl[i] != NULL) {
//			for (j = 0, np = nl[i]; np != NULL; np = np->next, ++j) {
//				printf("test %d\n", nl[i]);
//				printf("%s %s\n", np->name, np->defn);
//				last[j] = np;
//			}
//			while (j--) {
//				printf("j --> %d\n", last[j]);
//				//free(&last[j]);
//			}
//		}
//	}
//}

