#include "structs.c"
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>

static Screen screen;

int get_dimentions(Screen *sc)
{
	struct winsize win;
	if ((ioctl(0, TIOCGWINSZ, &win)) == -1)
		return 0;
	sc->col = win.ws_col;
	sc->row = win.ws_row;
	sc->len = sc->col * sc->row;
	return 1;
}

void blank_screen(Screen *sc)
{
	int i;

	for (i = 0; i < sc->row; i++)
		*(sc->display+i) = '\n';
	*(sc->display+i) = '\0';
}

char *advance_to(struct Window *file, size_t line)
{
	size_t i, j;
	char *fp;
	static size_t lines;
	fp = file->content;

	switch (line)
	{
		case 0: lines = 0;
			break;
		case 1: if (lines >= (unsigned)screen.row)
				lines -= screen.row;
			break;
		case 2: if (lines < (file->lines)-screen.row)
				lines += screen.row;
			break;
		default:
			break;
	}

	for (i = 0i, j = 0; i < file->len && j < lines; i++)
		if (*(fp++) == '\n')
			j++;
	return fp;
}

void draw_file(struct Window *file, size_t line)
{
	size_t i, j;
	char *f_pt, *d_pt;
	d_pt = screen.display;
	/* TODO Pege numbering system required */
	f_pt = advance_to(file, line);

	for (i = 0, j = 0; i < screen.len; i++)
		if (j < (unsigned)screen.row-1) {
			*d_pt++ = *f_pt++;
			if (*f_pt == '\n')
				j++;
		} else if (j < (unsigned)screen.row-1)
			*d_pt++ = '\n', j++;
	*d_pt++ = '\n';
	*d_pt = '\0';
}

void blit_screen(void)
{
	printf("%s", screen.display);
}

void init_screen(void)
{
	if (!(get_dimentions(&screen)))
		printf("error:	get_dimentions failed in init_screen\n");

	screen.display = malloc((screen.col * screen.row * sizeof(char))+1);
	blank_screen(&screen);
	blit_screen();
}

void free_screen(void)
{
	free(screen.display);
}

