#ifndef HEADER_FILE
#define HEADER_FILE

#include <unistd.h>
#include <fcntl.h>
#include <stddef.h>
#include <sys/stat.h>

typedef struct {
	int fd;
	uint8_t buf[40];
	int offset;
} myfile;

myfile *myopen(const char *pathname, int flags, mode_t mode );
ssize_t myread(myfile *file, void *buf, size_t count);
ssize_t mywrite(myfile *file, const void *buf, size_t count);
int myclose(myfile *file);

#endif
