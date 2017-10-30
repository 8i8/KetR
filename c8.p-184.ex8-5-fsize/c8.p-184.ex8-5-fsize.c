/*
 * Exercise 8-5. Modify the fsize program to print the other information
 * contained in the inode entry.
 *
 * https://www.gnu.org/software/libc/manual/html_node/Opening-a-Directory.html
 *
 * see getdents()
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>		/* flags for read and write */
#include <linux/types.h>	/* typedefs */
#include <linux/stat.h>		/* structures returned by stat */
#include "dirent.h"

void fsize(char *);

/* print file sizes */
int main(int argc, char* argv[])
{
	if (argc == 1)
		fsize(".");
	else
		while (--argc > 0)
			fsize(*++argv);
	return 0;
}

int stat(char *, struct stat *);
void dirwalk(char *, void (*fcn)(char *));

/* fsize:	print size of file "name" */
/*
   gdb ptype stbuf
	type = struct stat {
	    __dev_t st_dev;
	    __ino_t st_ino;
	    __nlink_t st_nlink;
	    __mode_t st_mode;
	    __uid_t st_uid;
	    __gid_t st_gid;
	    int __pad0;
	    __dev_t st_rdev;
	    __off_t st_size;
	    __blksize_t st_blksize;
	    __blkcnt_t st_blocks;
	    struct timespec st_atim;
	    struct timespec st_mtim;
	    struct timespec st_ctim;
	    __syscall_slong_t __glibc_reserved[3];
	}

	type = struct timespec {
	    __time_t tv_sec;
	    __syscall_slong_t tv_nsec;
	}
 */
void fsize(char *name)
{
	struct stat stbuf;

	if (stat(name, &stbuf) == -1) {
		fprintf(stderr, "fsize: can't access %s.\n", name);
		return;
	}
	if ((stbuf.st_mode & S_IFMT) == S_IFDIR)
		dirwalk(name, fsize);

	printf("%s:\n", name);
	printf("\tst_dev   	%10ld\n", stbuf.st_dev);
	printf("\tst_ino   	%10ld\n", stbuf.st_ino);
	printf("\tst_nlink 	%10ld\n", stbuf.st_nlink);
	printf("\tst_mode 	%10d\n", stbuf.st_mode);
	printf("\tst_uid   	%10d\n", stbuf.st_uid);
	printf("\tst_gid   	%10d\n", stbuf.st_gid);
	printf("\t__pad0   	%10d\n", stbuf.__pad0);
	printf("\tst_rdev 	%10lu\n", stbuf.st_rdev);
	printf("\tst_size 	%10ld\n", stbuf.st_size);
	printf("\tst_blksize 	%10ld\n", stbuf.st_blksize);
	printf("\tst_blocks 	%10ld\n", stbuf.st_blocks);
	printf("\tst_atim 	%10ld\n", stbuf.st_atim.tv_sec);
	printf("\tst_mtim 	%10ld\n", stbuf.st_mtim.tv_sec);
	printf("\tst_ctim 	%10ld\n", stbuf.st_ctim.tv_sec);
	printf("\n");
}

#define MAX_PATH	1024

/* dirwalk:	apply fcn to all files in dir */
void dirwalk(char *dir, void (*fnc)(char *))
{
	char name[MAX_PATH];
	Dirent *dp;
	DIR *dfd;

	if ((dfd = opendir(dir)) == NULL) {
		fprintf(stderr, "dirwalk: can't open %s.\n", dir);
		return;
	}
	while ((dp = readdir(dfd)) != NULL) {
		printf("inside %s\n", __func__);
		if (strcmp(dp->name, ".") == 0
		 || strcmp(dp->name, "..") == 0)
			continue;	/* skip self and parent */
		if (strlen(dir) + strlen(dp->name) + 2 > sizeof(name))
			fprintf(stderr, "dirwalk: name %s/%s too long\n",
					dir, dp->name);
		else {
			sprintf(name, "%s/%s", dir, dp->name);
			printf("test:%s\n", name);
			(*fnc)(name);
		}
	}
	closedir(dfd);
}

int fstat(int fd, struct stat *);

/* opendir:	open a directory for readdir calls */
DIR *opendir(char *dirname)
{
	int fd;
	struct stat stbuf;
	DIR *dp;

	if (
			(fd = open(dirname, O_RDONLY, 0)) == -1
			|| fstat(fd, &stbuf) == -1
			|| (stbuf.st_mode & S_IFMT) != S_IFDIR
			|| (dp = (DIR *) malloc(sizeof(DIR))) == NULL)
		return NULL;
	dp->fd = fd;
	return dp;
}

/* closedir:	close directory opened by opendir */
void closedir(DIR *dp)
{
	if (dp) {
		close(dp->fd);
		free(dp);
	}
}

#include "dir.h"	/* local directory structure */

/* readdir:	read directory entries in sequence */
Dirent *readdir(DIR *dp)
{
	struct direct dirbuf;	/* local directory structure */
	static Dirent d;	/* return: portable structure */

	/* TODO This read statment fails, returning -1 */
	while ((read(dp->fd, (char *) &dirbuf, sizeof(dirbuf)))
		       	== sizeof(dirbuf)) {
		if (dirbuf.d_ino == 0)	/* slot not in use */
			continue;
		d.ino = dirbuf.d_ino;
		strncpy(d.name, dirbuf.d_name, DIRSIZ);
		d.name[DIRSIZ] = '\0';	/* ensure termination */
		return &d;
	}

	//while ((getdirentries(dp->fd, (char *) &dirbuf, sizeof(dirbuf), 0))
	//	       	== sizeof(dirbuf)) {
	//	if (dirbuf.d_ino == 0)	/* slot not in use */
	//		continue;
	//	d.ino = dirbuf.d_ino;
	//	strncpy(d.name, dirbuf.d_name, DIRSIZ);
	//	d.name[DIRSIZ] = '\0';	/* ensure termination */
	//	return &d;
	//}
	printf("inside %s\n", __func__);
	return NULL;
}

