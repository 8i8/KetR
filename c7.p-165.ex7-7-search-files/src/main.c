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
	if (argc > 1)
		settings(argc, argv);

	/* If no files are supplied, fill lineptr array from stdin. */
	if (folio.count)
		folio = readfolio(folio);
	else
		printf("usage:	%s <file1> <file2>\n", *argv);
	
	printtest(folio);

	/* Sort input */
	//sortsection(lineptr, 0, nlines-1, state.func, 0);
	
//	/* If required add line spacers. */
//	if (state.directory)
//		nlines = addspacer(lineptr, MAXLINES, nlines, 0);
//
//	/* Sort using argv */
//	while (++i < (unsigned int)argc) {
//		settings(i, argv);
//
//		/* Sort input */
//		if (state.rsort)
//			sortsection(lineptr, 0, nlines-1, state.func, i-1);
//
//		nlines = sortdivide(lineptr, state.func, nlines, i-1);
//
//		/*
//		 * If directory setting is used, add a blank line break
//		 * after each new starting letter.
//		 */
//		if (state.directory)
//			nlines = addspacer(lineptr, MAXLINES, nlines, i-1);
//	}
//
//	/*
//	 * Output to terminal.
//	 */
//	writelines(lineptr, nlines);

	return 0;
}

/*
 * settings:	If the argument is preceded by '-' it is a flag else it is a
 * string, in which case it is sent to getinput() to be oped or recorded.
 */
size_t settings(int argc, char*argv[])
{
	size_t file;
	file = 0;

	resetglobals();

	while (--argc > 0)
		if (*argv[argc] == '-')
			getflags(argc, argv);
		else
			getinput(argv[argc], ++file);
	return file;
}

/*
 * sortsection:	Switch, selects the sort function for qsort.
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

