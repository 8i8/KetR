/*
 * Exercise 8-5. Modify the fsize program to print the other information
 * contained in the inode entry.
 *
 * http://man7.org/linux/man-pages/man2/getdents.2.html
 */
#define NAME_MAX	14	/* Longest filename compomponent */
				/* System dependant */
typedef struct {		/* Portable directory entry: */
	long ino;		/* inode nunmber */
	char name[NAME_MAX+1];	/* name + '\0' terminator */
} Dirent;

typedef struct {		/* Minimal DIR: no buffering etc. */
	int fd;			/* file descriptor for directory */
	Dirent d;		/* The directory entry */
} DIR;

DIR *opendir(char *dirname);
Dirent *readdir(DIR *dfd);
void closedir(DIR *dfd);

///*
// * System call to stat deals with the inode information, provided by system.
// */
//char* name;
//struct stat stbuf;
//int stat(char *, struct stat *);
//
////stat(name, &stbuf);
//
///* fould in sys/stat.h */
//struct stat			/* inode information returned by stat */
//{
//	dev_t	st_dev;		/* device of inode */
//	ino_t	st_ino;		/* inode number */
//	short	st_mode;	/* mode bits */
//	short	st_nlink;	/* number of linkes to file */
//	short	st_uid;		/* owners user id */
//	short	st_gid;		/* owners groouup id */
//	dev_t	st_rdev;	/* for special files */
//	off_t	st_size;	/* file size in characters */
//	time_t	st_atime;	/* time last accessed */
//	time_t	st_mtime;	/* time last modified */
//	time_t	st_ctime;	/* time inode last changed */
//};
//
///* defined in fcntl.h */
//#define S_IFMT	0160000	/* type of file */
//#define S_IFDIR	0040000	/* directory */
//#define S_IFCHR	0020000	/* character special */
//#define S_IFBLK	0060000	/* block special */
//#define S_IFREG	0100000	/* regular */
//
