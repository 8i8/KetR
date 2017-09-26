#include <stdio.h>
#include <stdlib.h>

struct Folio *init_folio(int num);
struct Folio *scan_files(struct Folio *book, char* file_name, int num_of_files);
void free_folio(struct Folio *book, size_t num);
void print_folio(struct Folio *f);
void init_screen(void);
void free_screen(void);
void blit_screen(struct Folio *file);
void refresh_screen(void);
