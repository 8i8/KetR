/*
 * Exercise 7-7. Modify the pattern finding program of Chapter 5 to take its
 * input from a set of named files or, if no files are named as arguments, from
 * the standard input. Should the file name be printed when a matching line is
 * found?
 * TODO
 */

/* Redefine getline */
#undef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define MAXFILES	10
#define MAXLEN		1000			/* Max length of input line */
#define MAXLINES	5000			/* Lines to be sorted */
#define ALLOCSIZE	5000000		/* Size of available space */
#define MAXTOKEN	6			/* Maximum number of tokens */
#define TOKENSIZE	24			/* Size of space for input tokens */

enum boolean { false, true };
enum function { simple, alpha, fold, nosort };

/* Function pointers */
typedef int (*comp)(void *, void *);		/* Sort functions for qsort */

/* Main */
void settings(int argc, char*argv[]);
void inputargs(int argc, char*argv[]);
void sortsection(char *lineptr[], int left, int right, int func, int ntab);
void globalreset(void);

/* i/o */
size_t readlines(char *lineptr[], size_t maxlines);
void writelines(char *lineptr[], size_t nlines);
size_t deleteline(char *lineptr[], int line, size_t nlines);
void settabs(char n[]);
size_t insertline(char *lineptr[], char* line, size_t maxlines, size_t index, size_t nlines);

/* Sort */
void _qsort(void *lineptr[], int left, int right, comp fn, int ntab);
size_t sortdivide(char *lineptr[], int func, size_t nlines, int ntab);
size_t addspacer(char *lineptr[], size_t maxlines, size_t nlines, int ntab);

/* Sort functions */
void swap(void *v[], size_t i, size_t j);
char* jumptochar(char *c);
char* jumptotab(char *c, int ntab);
int sortAlpha(char *s1, char *s2);
int sortAlphaCase(char *s1, char *s2);
int numcmp(char *s1, char *s2);
int strtcmp(char *s, char *t);

/* Function pointers */
extern comp strsimp;
extern comp stnsort;
extern comp strfold;

/* Global Memory */
extern char *lineptr[MAXLINES];			/* Pointer to text lines */
extern char allocbuf[ALLOCSIZE];		/* Storage for alloc */
extern char *allocp;					/* Next free position */

typedef short int bool;

/* Global flags */
typedef struct {
	bool numeric;			/* use numeric sort in qsort */
	bool reverse;			/* reverse search order */
	bool remempty;
	bool directory;
	bool resort;
	bool indx;
	bool linenum;
	int  func;				/* Define which function to use */
} State;

extern State state;

/* Global data struct, to store each file */

typedef struct {
	char *ln;
	size_t len;
} Line;

typedef struct {
	FILE *fp;
	char *name;
	Line **line;
} File;

extern File folio[];

