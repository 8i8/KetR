/*
 * Exercise 7-8. Write a program to print a set of files, starting each new one
 * on a new page, with a title and a running page count for each file.
 */
#include "structs.c"

#define NUMBER_BOOKS	10

int main(int argc, char *argv[])
{
	struct Folio *files;
	int i, c, flags;

	files = init_folio(NUMBER_BOOKS);

	for (i = 1, flags = 0; i < argc; i++)
		if (*argv[i] == '-') {
			c = *++argv[i], flags++;
			switch (c) {
				case 'n': printf("Hello World.");
					break;
				default:
					printf("usage: %s <file1> <file2> ...\n", argv[0]);
			}
		} else
			if ((files = scan_files(files, argv[i], NUMBER_BOOKS)) == NULL)
				printf("usage: %s <file1> <file2> ...\n", argv[0]);

	init_screen();
	blit_screen(&files[0]);
	refresh_screen();

	free_folio(files, argc-1-flags);
	free_screen();

	return 0;
}

