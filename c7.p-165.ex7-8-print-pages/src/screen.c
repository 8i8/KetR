#include "structs.c"
#include "print-files.h"
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

void blit_screen(struct Folio *file)
{
	size_t i, j;
	char *f_pt, *d_pt;
	f_pt = file->content;
	d_pt = screen.display;

	for (i = 0, j = 0; i < screen.len; i++) {
		if (i < file->len && j < file->lines) {
			*d_pt++ = *f_pt++;
			if (*f_pt == '\n')
				j++;
		} else if (j < (unsigned)screen.row)
			*d_pt++ = '\n', j++;
	}
	*d_pt = '\0';
}

void refresh_screen(void)
{
	printf("%s", screen.display);
}

void init_screen(void)
{
	if (!(get_dimentions(&screen)))
		printf("error:	get_dimentions failed in init_screen\n");

	screen.display = malloc((screen.col * screen.row * sizeof(char))+1);
	blank_screen(&screen);
	refresh_screen();
}

void free_screen(void)
{
	free(screen.display);
}

