/*
 * Exercise 8-3. Design and write _flushbuf, fflush, and fclose.
 */
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "syscalls.h"
#define PERMS 0666	/* RW for owner, group, others */

#undef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200112L

FILE _iob[OPEN_MAX] = {		/* stdin, stdout, stderr */
	{ 0, (char *) 0, (char *) 0, 0, { 1, 0, 0, 0, 0 }, 0 },
	{ 0, (char *) 0, (char *) 0, 0, { 0, 1, 0, 0, 0 }, 1 },
	{ 0, (char *) 0, (char *) 0, 0, { 0, 1, 1, 0, 0 }, 2 }
};

FILE *fopen(char *name, char *mode)
{
	int fd;
	FILE *fp;

	if (*mode != 'r' && *mode != 'w' && *mode != 'a')
		return NULL;
	for (fp = _iob; fp < _iob + OPEN_MAX; fp++)
		if ((fp->flags._read == 0 && fp->flags._write == 0))
			break;	/* found free slot */
	if (fp >= _iob + OPEN_MAX)	/* no free slots */
		return NULL;

	if (*mode == 'w')
		fd = creat(name, PERMS);
	else if (*mode == 'a') {
		if ((fd = open(name, O_WRONLY, 0)) == -1)
			fd = creat(name, PERMS);
		lseek(fd, 0L, 2);
	} else
		fd = open(name, O_RDONLY, 0);
	if (fd == -1)	/* couldn't access name */
		return NULL;
	fp->fd = fd;
	fp->cnt = 0;
	fp->base = NULL;
	fp->flags._read = (*mode == 'r') ? 1 : 0;
	fp->flags._write = (*mode == 'w') ? 1 : 0;
	return fp;
}

/* _fillbuf:	allocate and fill input buffer */
int _fillbuf(FILE *fp)
{
	int bufsize;

	if (!(fp->flags._read && !(fp->flags._eof) && !(fp->flags._err)))
		return EOF;
	bufsize = !(fp->flags._unbuf) ? 1 : BUFSIZ;
	if (fp->base == NULL)	/* no buffer yet */
		if ((fp->base = (char *)malloc(bufsize)) == NULL)
			return EOF;	/* can't get buffer */
	fp->ptr = fp->base;
	fp->cnt = read(fp->fd, fp->ptr, bufsize);
	if (--fp->cnt < 0) {
		if (fp->cnt == -1)
			fp->flags._eof = 1;
		else
			fp->flags._err = 1;
		fp->cnt = 0;
		return EOF;
	}
	return (unsigned char) *fp->ptr++;
}

int _flushbuf(int a, FILE *fp)
{
	return a;
}

int main (int argc, char *argv[])
{
	FILE *fp;
	int i;
	char c[BUFSIZ], *c_pt;
	c_pt = c;

	fp = fopen(argv[--argc], "r");

	while (!feof(fp))
	{
		for (i = 0; i < BUFSIZ && ((*c_pt = getc(fp)) != EOF); i++)
			c_pt++;
		write(1, c, i);
		c_pt = c;
	}

	return 0;
}


