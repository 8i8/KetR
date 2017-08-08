/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  init.c ~ initiate variables
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
#include "search-files.h"

/* main:	portfolio of file input */
Folio folio = { { {
		{
			NULL,
		},
		NULL,
		NULL,
		0,
		0,
		0
	} },
	0,
	0
};

/* main:	state for program functions */
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

/*
 * Values that are reset between argv strings.
 */
void resetglobals(void)
{
	state.numeric = 	false;
	state.reverse = 	false;
	state.remempty =	false;
	state.directory =	false;
	state.rsort = 		false;
	state.indx =		false;
}

char *memory;

