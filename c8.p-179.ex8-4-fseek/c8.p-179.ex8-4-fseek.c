/*
 * Exercise 8-4. The standard library function int fseek(FILE *fp, long offset,
 * int origin) is identical to lseek except that fp is a file pointer instead
 * of a file descriptor and return value is an int status, not a position.
 * Write fseek. Make sure that your fseek coordinates properly with the
 * buffering done for the other functions of the library.
 */
#include <stdlib.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include <error.h>
#include "syscalls.h"
#define PERMS 0666	/* RW for owner, group, others */

#undef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200112L

FILE _iob[OPEN_MAX] = {		/* stdin, stdout, stderr */
	{ 0, (char *) 0, (char *) 0, _READ, 0 },
	{ 0, (char *) 0, (char *) 0, _WRITE, 1 },
	{ 0, (char *) 0, (char *) 0, _WRITE | _UNBUF, 2 }
};

/**
 * init_iob:	Initialise stdin stdout and stderr, this is usually done by the
 * operating system, however having been redefined here they consequently
 * require initialization.
 */
void init_iob()
{
	_iob[0].base = _iob[0].ptr = malloc(BUFSIZ);
	_iob[1].base = _iob[1].ptr = malloc(BUFSIZ);
	_iob[2].base = _iob[2].ptr = malloc(1);
};

/**
 * fopen:	Open file and load into FILE struct.
 */
FILE *fopen(char *name, char *mode)
{
	int fd;
	FILE *fp;

	if (mode != NULL && *mode != 'r' && *mode != 'w' && *mode != 'a')
		return NULL;
	for (fp = _iob; fp < _iob + OPEN_MAX; fp++)
		if ((fp->flag & (_READ | _WRITE)) == 0)
			break;	/* found free slot */
	if (fp >= _iob + OPEN_MAX)	/* no free slots */
		return NULL;
	if (*mode == 'w')
		fd = creat(name, PERMS);
	else if (*mode == 'a') {
		if ((fd = open(name, O_WRONLY, SEEK_SET)) == -1)
			fd = creat(name, PERMS);
		lseek(fd, 0L, SEEK_END);
	} else
		fd = open(name, O_RDONLY, SEEK_SET);
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

	if((fp->flag & _WRITE) && (fp->flag & _UNBUF))
		write(1, c, 1);
	else if (fp->flag & _WRITE) {
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
 * check_negative_offset:	Returns -1 when a negative offset extends
 * further than the beggining of the file. Returns the length of the file in
 * the case that SEEK_END has been requested, to negate performing the same
 * operation twice.
 */
int check_negative_offset(FILE *fp, long offset, int origin)
{
	long int cur, len;

	if (origin == SEEK_SET) {
		if (offset < 0)
			return -1;
	} else if (origin == SEEK_CUR) {
		if (offset < 0 && offset < -(fp->ptr - fp->base))
			return 1;
	} else if (origin == SEEK_END) {
		cur = lseek(fp->fd, 0, SEEK_CUR);
		len = lseek(fp->fd, 0, SEEK_END);
		/* If negative file length is bigger than the negative offset */
		if (-len > offset) {
			lseek(fp->fd, cur, SEEK_SET);
			return -1;
		} else
			return len;
	}
	return 0;
}

/**
 * fseek_offset:	Check that the value of the offset does not go out of
 * the bounds of the current buffer.
 */
//int fseek_offset()
//{
//}

/**
 * fseek:	Wrapper arround the posix lseek() function.
 */
int fseek(FILE *fp, long offset, int origin)
{
	long int len;
	len = 0;

	if (fp->flag & (_UNBUF | _ERR | _EOF) ||
			(len = check_negative_offset(fp, offset, origin)) < 0) {
		error(0, 0, "error: check_negative_offset failed in %s.", __func__);
		return -1;
	}

	if (origin == SEEK_SET && !(fp->flag & _UNBUF))
		fp->ptr = fp->base+offset, fp->cnt = 0;
	else if (origin == SEEK_CUR && !(fp->flag & _UNBUF))
		if (len)
			fflush(fp);
		else
			fp->ptr += offset, fp->cnt -= offset;
	else if (origin == SEEK_END && !(fp->flag & _UNBUF)) {
		fp->ptr = fp->base + len, fp->cnt = 0;
		return 0;
	} else
		return -1;

	if (!(lseek(fp->fd, offset, origin) >= 0))
		return -1;

	return 0;
}

/**
 * main:	Test code for the _flushbuf, fflush, fclose and fseek functions.
 */
int main (int argc, char *argv[])
{
	init_iob(); /* Imitates the systems opening of stdin stdout and stderr */

	FILE *fp;
	int c, i;
	i = 1;

	if ((fp = fopen(argv[--argc], "r")) != NULL) {

		while ((c = getc(fp)) != EOF) {
			putchar(c);
			i++;
			if (i == 5) {
				//write(1, "\nHere\n", 6);
				if (fseek(fp, -7, SEEK_SET))
					error(0, 0, "error: fseek failed in %s.", __func__);
			}
		}

		fclose(fp);
	}

	free_iob();

	return 0;
}

