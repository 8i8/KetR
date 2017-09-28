#include "structs.c"

static int num_of_files;

struct Window *init_folio(int num)
{
	struct Window *define_folio(struct Window *folio);
	struct Window *book, *pt;
	size_t i;
	num_of_files = num;

	if ((pt = book = malloc(num * sizeof(struct Window))) == NULL)
		printf("error: malloc failed in init_folio().\n");

	for (i = 0; i < (unsigned)num; i++, book++)
		book = define_folio(book);

	return pt;
}

struct Window *define_folio(struct Window *folio)
{
	folio->fp = NULL;
	folio->name = NULL;
	folio->lines = 0;
	return folio;
}

static size_t file_size(FILE *fp)
{
	size_t len;
	fseek(fp, 0L, SEEK_END);
	len = 1 + (unsigned)ftell(fp);
	rewind(fp);
	return len;
}

/*
 * read_file:	Transfer file onto programs heap memory.
 */
int read_file(struct Window *folio)
{
	int c, d;
	char* pt;

	if (folio->fp == NULL) {
		printf("error:	fp recieved by read_file() is NULL.\n");
		return 1;
	}

	folio->len = file_size(folio->fp);
	folio->content = pt = malloc((folio->len * sizeof(char))+1);

	while (c != EOF) {
		d = c, c = fgetc(folio->fp);
		*pt++ = c;
		if (c == '\n')
			folio->lines++;
	}
	if (d != '\n')
		folio->lines++;

	*pt = '\0';

	fclose(folio->fp);
	return 0;
}

/*
 * scan_files:	Treat every file in input list.
 */
struct Window *scan_files(struct Window *book, char* file_name, int num_of_files)
{
	static int b_pt;
	
	if (b_pt < num_of_files) {
		if ((book[b_pt].fp = fopen(file_name, "r")) == NULL)
			return NULL;
		book[b_pt].name = file_name;
		if (read_file(&book[b_pt++]))
			printf("error:	read_file error in scan_files.\n");
	} else
		printf("error: to many files for current configuration.\n");

	return book;
}

void free_folio(struct Window *files, size_t num)
{
	size_t i;
	for (i = 0; i < num; i++)
		free(files[i].content);
	free(files);
}

