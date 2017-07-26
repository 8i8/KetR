/*
 * Exercise 7-2. Write a program that will print arbitrary input in a sensible
 * way. As a minimum, it should print non-graphic characters in octal or
 * hexadecimal according to local custom, and break long text lines.
 *
 * TODO This question has not yet been compleeted.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLEN		1000
#define MAXPAGE		1000
#define MAXBUF		100

typedef struct pagearrary {
	char *p[MAXPAGE];
	size_t pt;
} Page;

static int _getline(char *s, size_t len);
static Page storeline(Page page, char *s);
static void printpage(Page page);
static void freepage(Page page);
static int getch(void);
static void ungetch(char c);

int main(void)
{
	Page page = { 0 };
	char s[MAXLEN] = {'\0'};

	while ((_getline(s, MAXLEN)) != EOF)
		page = storeline(page, s);

	printpage(page);
	freepage(page);

	return 0;
}

//static int mprintf(char *s, size_t len)
//{
//	size_t i = 0;
//
//	for (i = 0; i < len; i++) {
//	}
//	return 0;
//}

static int _getline(char *s, size_t len)
{
	int c;
	size_t i = 0;

	for(i = 0; i < len && (c = getch()) != EOF; i++)
		*s++ = c;

	return EOF;
}

/*
 * storeline:	add line to page array
 */
static Page storeline(Page page, char *s)
{
	char *line;

	if (page.pt < MAXPAGE) {
		line = (char*) malloc(strlen(s)*sizeof(char));
		strcpy(line, (char*)s);
		page.p[page.pt++] = line;
	}
	return page;
}

/*
 * printpage:	print entire page array
 */
static void printpage(Page page)
{
	size_t i;
	for (i = 0; i < page.pt; i++)
		printf("%s", page.p[i]);
}

/*
 * freepage:	free all memory 
 */
static void freepage(Page page)
{
	size_t i;
	for (i = 0; i < page.pt; i++)
		free(page.p[i]);
}


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Get int
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
static int buf[MAXBUF];
static int p_buf = 0;

static int getch(void)
{
	return (p_buf > 0) ? buf[--p_buf] : getchar();
}

static void ungetch(char c)
{
	if (p_buf < MAXBUF)
		buf[p_buf++] = c;
	else
		printf("ungetch:	error, stack full.\n");
}

