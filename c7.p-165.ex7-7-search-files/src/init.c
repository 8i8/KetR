/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  init.c ~ initiate variables
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
#include "search-files.h"

char *lineptr[MAXLINES];		/* Pointer to text lines */
char allocbuf[ALLOCSIZE];		/* Storage for alloc */
char *allocp = allocbuf;		/* Next free position */

/* Function pointers */
comp strsimp = (int (*)(void*, void*)) strcmp;
comp stnsort = (int (*)(void*, void*)) sortAlpha;
comp strfold = (int (*)(void*, void*)) sortAlphaCase;

State state = {
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	alpha
};

File folio[MAXFILES] = { {
	NULL,
	NULL,
	NULL
} };

/*
 * Values that are reset between argv strings.
 */
void globalreset(void)
{
	state.numeric = 	false;
	state.reverse = 	false;
	state.remempty =	false;
	state.directory =	false;
	state.resort = 		false;
	state.indx =		false;
}

