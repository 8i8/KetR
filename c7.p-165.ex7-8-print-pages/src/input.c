#include "structs.c"

void get_input(struct Window *files, int c)
{
	switch (c)
	{
		case START: draw_file(&files[0], START);
			  break;
		case 'k': draw_file(&files[0], UP);
			  break;
		case XK_Up: draw_file(&files[0], UP);
			  break;
		case 'j': draw_file(&files[0], DOWN);
			  break;
		case XK_Down: draw_file(&files[0], DOWN);
			  break;
		case 0x0A: draw_file(&files[0], DOWN);
			  break;
		default:
			  break;
	}
}
