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
	{ 0, (char *) 0, (char *) 0, _READ, 0 },
	{ 0, (char *) 0, (char *) 0, _WRITE, 1 },
	{ 0, (char *) 0, (char *) 0, _WRITE | _UNBUF, 2 }
};

void init_iob()
{
	short i;
	for (i = 0; i < 3; i++)
		_iob[i].base = _iob[i].ptr = malloc(BUFSIZ);
};

/**
 * fopen:	Open file and load into FILE struct.
 */
FILE *fopen(char *name, char *mode)
{
	int fd;
	FILE *fp;

	if (*mode != 'r' && *mode != 'w' && *mode != 'a')
		return NULL;
	for (fp = _iob; fp < _iob + OPEN_MAX; fp++)
		if ((fp->flag & (_READ | _WRITE)) == 0)
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
	fp->flag = (*mode == 'r') ? _READ : _WRITE;
	return fp;
}

/**
 * _fillbuf:	allocate and fill input buffer.
 */
int _fillbuf(FILE *fp)
{
	int bufsize;

	if ((fp->flag & (_READ | _EOF | _ERR)) != _READ)
		return EOF;
	bufsize = (fp->flag & _UNBUF) ? 1 : BUFSIZ;
	if (fp->base == NULL)	/* no buffer yet */
		if ((fp->base = (char *)malloc(bufsize)) == NULL)
			return EOF;	/* can't get buffer */
	fp->ptr = fp->base;
	fp->cnt = read(fp->fd, fp->ptr, bufsize);
	if (--fp->cnt < 0) {
		if (fp->cnt == -1)
			fp->flag |= _EOF;
		else
			fp->flag |= _ERR;
		fp->cnt = 0;
		return EOF;
	}
	return (unsigned char) *fp->ptr++;
}

/**
 * _flushbuf:
 */
int _flushbuf(int a, FILE *fp)
{
	char *pt, *c;
	c = (char*)&a;
	if((fp->flag & _WRITE) != 0) {
		if ((fp->ptr - fp->base) > 0 ) {
			pt = fp->base;
			write(1, pt++, fp->ptr - fp->base);
			fp->ptr = fp->base;
		}
		write(1, c, 1);
	} else
		return -1;

	return 0;
}

void free_fp(FILE *fp)
{
	if ((fp->flag & (_READ | _WRITE)) == 0)
		free(fp->base);
}

void free_iob(void)
{
	int i;
	for (i = 0; i < OPEN_MAX; i++)
		free_fp(&_iob[i]);
}

int main (int argc, char *argv[])
{
	FILE *fp;
	int c;

	init_iob();
	fp = fopen(argv[--argc], "r");

	while ((c = getc(fp)) != EOF)
		putchar(c);

	free_iob();

	return 0;
}

