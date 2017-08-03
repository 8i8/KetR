/*
 * Exercise 7-7. Modify the pattern finding program of Chapter 5 to take its
 * input from a set of named files or, if no files are named as arguments, from
 * the standard input. Should the file name be printed when a matching line is
 * found?
 */
#include "search-files.h"

/*
 * Sort input lines.
 */
int main(int argc, char *argv[])
{
	size_t nlines;		/* number of input lines to read */
	int i;


	/* If there are arguments entered, read argv[1] */
	i = 1;
	if (argc > i)
		settings(argc, argv);

	/*
	 * Fill lineptr array from stdin.
	 */
	if (!(nlines = readlines(lineptr, MAXLINES))) {
		printf("Error: input to big to sort\n");
		return 1;
	}
	
	/* Sort input */
	sortsection(lineptr, 0, nlines-1, state.func, 0);
	
	/* If required add line spacers. */
	if (state.directory)
		nlines = addspacer(lineptr, MAXLINES, nlines, 0);

	/* Sort using argv */
	while (++i < argc) {
		settings(i, argv);

		/* Sort input */
		if (state.resort)
			sortsection(lineptr, 0, nlines-1, state.func, i-1);

		nlines = sortdivide(lineptr, state.func, nlines, i-1);

		/*
		 * If directory setting is used, add a blank line break
		 * after each new starting letter.
		 */
		if (state.directory)
			nlines = addspacer(lineptr, MAXLINES, nlines, i-1);
	}

	/*
	 * Output to terminal.
	 */
	writelines(lineptr, nlines);

	return 0;
}

/*
 * Set state before running qsort.
 */
void settings(int argc, char*argv[])
{
	FILE *fp;
	int c, i, file;
	file = i = 0;
	globalreset();

	while (--argc > 0)
	{
		if (*argv[argc] == '-')
		{
			while ((c = *++argv[argc])) {
				switch (c) {
					case 'a':
						state.func = alpha;
						break;
					case 'd':
						state.indx = true;
						state.directory = true;
						break;
					case 'e':
						state.remempty = true;
						break;
					case 'f':
						state.func = fold;
						break;
					case 'i':
						state.indx = true;
						break;
					case 'n':
						state.indx = true;
						state.numeric = true;
						break;
					case 'N':
						state.linenum = true;
						break;
					case 'p':
						state.func = nosort;
						break;
					case 'r':
						state.reverse = true;
						break;
					case 's':
						state.func = simple;
						break;
					case 'R':
						state.resort = true;
					default:
						break;
				}
			}
		}
		/* Input files if address given */
		else if ((fp = fopen(argv[i], "r")) != NULL)
			folio[file].fp = fp, folio[file].name = argv[argc];
		else
			folio[file].fp = NULL, folio[file].name = argv[argc];
		i++, file++;
	}
}

/*
 * Switch, selects the sort function for qsort.
 */
void sortsection(char *lineptr[], int left, int right, int func, int ntab)
{
	switch (func) {
		case simple:
			_qsort((void **) lineptr, left, right, strsimp, ntab);
			break;
		case alpha:
			_qsort((void **) lineptr, left, right, stnsort, ntab);
			break;
		case fold:
			_qsort((void **) lineptr, left, right, strfold, ntab);
			break;
		case nosort:
			break;
		default:
			break;
	}
}

