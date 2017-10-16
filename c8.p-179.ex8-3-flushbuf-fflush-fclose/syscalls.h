/*
 * Exercise 8-2. Rewrite fopen and _fillbuf with fields instead of explicit bit
 * operations. Compare code size and execution speed.
 */
//#define NULL		0
#define EOF		(-1)
#define BUFSIZ		1024
#define OPEN_MAX	20

typedef struct {
	unsigned int _read	: 1;	/* file open for reading */
	unsigned int _write	: 1;	/* file open for writing */
	unsigned int _unbuf	: 1;	/* file is unbuffered */
	unsigned int _eof	: 1;	/* EOF has occured on this file */
	unsigned int _err	: 1;	/* error occured on this file */
} Flags;

typedef struct _iobuf {
	int cnt;	/* characters left */
	char *ptr;	/* next character position */
	char *base;	/* location of buffer */
	int flag;	/* mode of file access */
	Flags flags;
	int fd;		/* file descriptor */
} FILE;
extern FILE _iob[OPEN_MAX];

#define stdin	(&_iob[0])
#define stdout	(&_iob[1])
#define stderr	(&_iob[2])

//enum _flags {
//	_READ	= 01,	/* file open for reading */
//	_WRITE	= 02, 	/* file open for writing */
//	_UNBUF	= 04,	/* file is unbuffered */
//	_EOF	= 010,	/* EOF has occured on this file */
//	_ERR	= 020	/* error occured on this file */
//};

int _fillbuf(FILE *);
int _flushbuf(int, FILE *);

#define feof(p)		(((p)->flags._eof) == 1)
#define ferror(p)	(((p)->flags->_err) == 1)
#define fileno(p)	((p)->fd)

#define getc(p)		(--(p)->cnt >= 0 \
		? (unsigned char) *(p)->ptr++ : _fillbuf(p))
#define putc(x,p)	(--(p)->cnt >= 0 \
		? *(p)->ptr++ = (x) : _flushbuf((x),p))

#define getchar()	getc(stdin)
#define putchar(x)	putc((x), stdout)

