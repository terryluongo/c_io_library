#include "myio.h"


myfile *myopen(const char *pathname, int flags,mode_t mode) {
	int fd;	
	fd = open(pathname,flags,mode);
	myfile *file;
	if ((file = malloc(sizeof *file)) != NULL) {
		file->fd = fd;
		file->offset = 0;
	}
	return file;
}
ssize_t myread(myfile *file, void *buf, size_t count) {
	ssize_t amountRead;	
	amountRead = read(file->fd, buf, count);
	return amountRead;
}
ssize_t mywrite(myfile *file, const void *buf, size_t count) {
	ssize_t bytes_written;
	bytes_written = write(file->fd, buf, count);
	return bytes_written;
}
int myclose(myfile *file) {
	int success;
	success = close(file->fd);
	free(file);
	return success;
}


