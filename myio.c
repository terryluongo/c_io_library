#include "myio.h"

#define buf_max 400
myfile *myopen(const char *pathname, int flags,mode_t mode) {
	int fd;	
	fd = open(pathname,flags,mode);
	myfile *file;
	if ((file = malloc(sizeof *file)) != NULL) {
		file->fd = fd;
		file->roffset = 0;
		file->woffset = 0;
	}
	return file;
}
ssize_t myread(myfile *file, void *buf, size_t count) {
	ssize_t amountRead;	
	// have to think about if read is called for an amount higher than the buffer 
	// or an overlap case where we have 100 bytes left in buffer and 200 are asked for 
	
	// successive calls have to start with a first read
	int buf_amount = 0;

	if (file->roffset + count >= buf_max) {
		printf("trigger flush\n");
		buf_amount = buf_max - file->roffset;
		memcpy(buf,file->rbuf + file->roffset,buf_amount);
		file->roffset = 0; 	
	}
	if (file->roffset == 0) {
		amountRead = read(file->fd, file->rbuf, buf_max);
	}

	//  regular old advance the buffer call
	
	memcpy((char *) buf + buf_amount,file->rbuf + file->roffset,count - buf_amount);
	file->roffset += count;
	return amountRead;
}
ssize_t mywrite(myfile *file, const void *buf, size_t count) {
	ssize_t bytes_written;
	memcpy(file->wbuf,buf,count);
	bytes_written = write(file->fd, file->wbuf, count);
	return bytes_written;
}
int myclose(myfile *file) {
	int success;
	success = close(file->fd);
	free(file);
	return success;
}


