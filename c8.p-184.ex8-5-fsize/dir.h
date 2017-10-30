/*
 * Exercise 8-5. Modify the fsize program to print the other information
 * contained in the inode entry.
 */
#ifndef DIRSIZ
#define DIRSIZ	14
#endif

struct direct	/* directory entry */
{
	ino_t	d_ino;		/* inode number */
	char	d_name[DIRSIZ];	/* long name does not have '\0' */
};
