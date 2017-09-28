#include <stdio.h>
#include <stdlib.h>

#define XK_Left                          0xff51  /* Move left, left arrow */
#define XK_Up                            0xff52  /* Move up, up arrow */
#define XK_Right                         0xff53  /* Move right, right arrow */
#define XK_Down                          0xff54  /* Move down, down arrow */

enum move { START, UP, DOWN };

struct Window *init_folio(int num);
struct Window *scan_files(struct Window *book, char* file_name, int num_of_files);
void free_folio(struct Window *book, size_t num);
void print_folio(struct Window *f);
void init_screen(void);
void free_screen(void);
void draw_file(struct Window *file, size_t line);
void blit_screen(void);
void get_input(struct Window *files, int c);
