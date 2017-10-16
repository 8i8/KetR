/*
 * Exercise 8-1. Rewrite the program cat from Chapter 7 using read, write,
 * open, and close instead of their standard library equivalents. Perform
 * experiments to determine the relative speeds of the two versions.
 */
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <error.h>
#include <stdint.h>

/**
 * cat:	concatinate files version 1.
 */
int main(int argc, char* argv[])
{
	char buf[BUFSIZ];
	int fd;
	size_t time = SIZE_MAX;

	void filecopy(int, char*, int);

	if (argc == 1) /* no args copy standard input */
		filecopy(read(0, buf, BUFSIZ), buf, BUFSIZ);
	else
		while (--argc > 0)
			if ((fd = open(*++argv, O_RDONLY, 0)) == -1) {
				error(0, 0, "cat: cant't open %s\n", *argv);
				return 1;
			} else {
				filecopy(fd, buf, BUFSIZ);
				close(fd);
			}
	return 0;
}

void filecopy(int fd, char* buf, int lim)
{
	int n;

	while ((n = read(fd, buf, lim)) > 0)
		write(1, buf, n);
}

