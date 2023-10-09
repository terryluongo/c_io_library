#ifndef HEADER_FILE
#define HEADER_FILE

#include <unistd.h>
#include <fcntl.h>
#include <stddef.h>
#include <sys/stat.h>

int myopen(const char *pathname, int flags, mode_t mode );
ssize_t myread(int fd, void *buf, size_t count);
ssize_t mywrite(int fd, const void *buf, size_t count);
int myclose(int fd);

#endif
