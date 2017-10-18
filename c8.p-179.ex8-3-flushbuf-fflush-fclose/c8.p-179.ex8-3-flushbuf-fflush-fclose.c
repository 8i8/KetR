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
	_iob[0].base = _iob[0].ptr = malloc(BUFSIZ);
	_iob[1].base = _iob[1].ptr = malloc(BUFSIZ);
	_iob[2].base = _iob[2].ptr = malloc(BUFSIZ);
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
	size_t len = fp->ptr - fp->base;

	if((fp->flag & _WRITE) && (fp->flag & _UNBUF)) {
		if ((fp->ptr - fp->base) > 0 ) {
			pt = fp->base;
			write(2, pt, len);
		}
		write(2, c, 1);
	}
	if (fp->flag & _WRITE) {
		if ((fp->ptr - fp->base) > 0 ) {
			pt = fp->base;
			write(1, pt, len);
		}
		write(1, c, 1);
	}

	fp->ptr = fp->base;
	return 0;
}

/**
 * fclose:
 */
int fclose(FILE *fp)
{
	if ((fp->flag & (_READ | _WRITE)) == 0)
		free(fp->base), close(fp->fd);
	else
		return -1;
	return 0;
}

/**
 * free_iob:	Used in conjuction with fclose() to release all FILE memory
 * allocated.
 */
int free_iob(void)
{
	if ((fclose(&_iob[0])) || (fclose(&_iob[1])) || (fclose(&_iob[2])))
		return -1;
	return 0;
}

/**
 * fflush:	Clear a files buffer.
 */
void fflush(FILE *fp)
{
	fp->ptr = fp->base;
	fp->cnt = 0;
}

/**
 * A program to test the use of a small FILE implementation, this requires that
 * <stdio.h> not be used. Note that in normal circumstances the files used
 * would be initialized by the system when the c program is run, here the
 * init_iob function does this for us, to replace the systems code.
 */
int main (int argc, char *argv[])
{
	FILE *fp;
	int c;
	init_iob(); /* Imitates the systems opening of stdin stdout and stderr */

	fp = fopen(argv[--argc], "r");

	while ((c = getc(fp)) != EOF)
		putchar(c);

	fclose(fp);
	write(2, "Done and dusted.\n", 17);
	free_iob();

	return 0;
}

