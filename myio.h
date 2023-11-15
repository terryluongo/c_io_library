#ifndef MYIO_HEADER //look at noes
#define MYIO_HEADER

#include <stdint.h>
#include <sys/stat.h>
#include <stdio.h>

typedef struct {
	int fd;
	uint8_t rbuf[400];
	int roffset;
	int rbufend;
	uint8_t wbuf[400];
	int woffset;
} myfile;

myfile *myopen(const char *pathname, int flags, mode_t mode );
ssize_t myread(myfile *file, void *buf, size_t count);
ssize_t mywrite(myfile *file, const void *buf, size_t count);
int myclose(myfile *file);
int myseek(myfile *file, int offset, int whence);
int myflush(myfile *file);

#endif
