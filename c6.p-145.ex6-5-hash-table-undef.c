/*
 * Exercise 6-5. Write a function undef that will remove a name and definition
 * from the table maintained by lookup and install.
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct nlist {				/* table entry */
	struct nlist *next;		/* next entry in chain */
	char *name;
	char *defn;
};

#define HASHSIZE 101
static struct nlist *hashtab[HASHSIZE];	/* pointer table */
static int undef(char *def);
static struct nlist *lookup(char *s);
static struct nlist *install(char *name, char *defn);

int main(void)
{
	char name[] = "TEST";
	char defn[] = "101";
	struct nlist *np;

	install(name, defn);

	np = lookup("TEST");
	printf("%s\n", np->defn);

	undef("TEST");

	np = lookup("TEST");

	if (np != NULL)
		printf("%s\n", np->defn);
	else
		printf("Value not defined in table.\n");

	return 0;
}

static int undef(char *def)
{
	struct nlist *np;

	np = lookup(def);

	if (np != NULL) {
		free(np->name);
		free(np->defn);
		return 1;
	} else
		return 0;
}

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
 * lookup:	Look for s in hashtab
 */
static struct nlist *lookup(char *s)
{
	struct nlist *np;

	for (np = hashtab[hash(s)]; np != NULL; np = np->next)
		if (strcmp(s, np->name) == 0)
			return np;	/* found */
	return NULL;			/* not found */
}

/*
 * install:	put (name, defn) in hashtab
 */
static struct nlist *install(char *name, char *defn)
{
	struct nlist *np;
	unsigned hashval;

	if ((np = lookup(name)) == NULL) {	/* not found */
		np = (struct nlist *) malloc(sizeof(*np));
		if (np == NULL || (np->name = strdup(name)) == NULL)
			return NULL;
		hashval = hash(name);
		np->next = hashtab[hashval];
		hashtab[hashval] = np;
	} else		/* already there */
		free((void *) np->defn);	/* free previous defn */
	if ((np->defn = strdup(defn)) == NULL)
		return NULL;
	return np;
}

