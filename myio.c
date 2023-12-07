#include "myio.h"

#include <unistd.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>


#define BUF_MAX 10 

// file has to be advanced forward the proper amount, 90 bytes
// do we do it when read is called again or before write returns?
// additionally, write is not delivering immediately, it itself has a buffer
// 
// keep track of total bytes requested for read, total bytes requested for write 
// if read == total_bytes, read behaves as usual
//
// if write == total_bytes behaves as usual
//
// if read < total_bytes, lseek(local, total_bytes - read); then after, lseek(local, total_bytes + read);
// then add read bytes
// same by symmetry
//
//don't even need total_bytes, just if the opposite = 0, you do nothing
//so just no matter what do lseek(local, -opposite_bytes); then lseek(local, opposite_bytes); and then add rw_bytes += count;
//
// so it would behave identically to expected with no overlap, and the rest of the function would be the same with overlap once we make initial adjustment, then just reset after 
//
// myseek
// 100 buffer
// read 10 bytes, write 20 bytes, lseek(local, -20); -> should go to 10 bytes
// but in my version it would be at 80 bytes, write hasn't delivered yet, read filled one buffer
// so needs to keep track of how many bytes actually have been delievered to the user and within the write buffer 
// could have number that keeps track of total amount of bytes actually asked for by the user, would implement this pretty well, even if it is relative search just hardcode asked for amount + relative 
// hardcode easy to implement 
myfile *myopen(const char *pathname, int flags,mode_t mode) {
	int fd;
	if ((fd = open(pathname, flags, mode)) == 0) {
		perror("open");
		exit(1);
	}
	myfile *file;
	if ((file = malloc(sizeof *file)) != NULL) {
		file->fd = fd;
		file->roffset = 0;
		file->woffset = 0;
		file->rbufend = -2;
	}
	return file;
}

ssize_t myread(myfile *file, void *buf, size_t count) {
	size_t returnCount = count;
	int buf_amount = 0;

	// act on sentinel value from before and return
	if (file->rbufend == -1) {
		return 0;
	}
		
	//check if call will empty buffer
	if ((file->rbufend != -2) && (file->roffset + count == BUF_MAX)) {
		if (file->rbufend - file->roffset < returnCount) {
			returnCount = file->rbufend - file->roffset;
		}
		
		//our syscall read less than the BUF_MAX, so file is empty, trigger end condition
		if (file->rbufend < BUF_MAX) {
			file->rbufend = -1;
		}
		memcpy(buf, file->rbuf + file->roffset, returnCount);
		file->roffset=0;
		
		return returnCount;
	}
	
	//check if call will exceed buffer
	else if (file->rbufend && file->roffset > 0 && file->roffset + count > BUF_MAX) {
		buf_amount = BUF_MAX - file->roffset;
		memcpy(buf,file->rbuf + file->roffset, buf_amount);
		file->roffset = 0; 	
	}

	//either first call or we need more stuff
	if (file->roffset == 0) {

		if (count >= BUF_MAX) {
			file->rbufend = read(file->fd, (char *) buf + buf_amount, count - buf_amount);
		}

		else {
			file->rbufend = read(file->fd, file->rbuf, BUF_MAX);
		}
		if (file->rbufend == 0) {
			memcpy(buf, file->rbuf + BUF_MAX - buf_amount, buf_amount);
			return buf_amount;
		}
	}

	// if what is left is less than what we asked for, trigger end condition	
	int copy_amount = count - buf_amount;
	if (file->rbufend - file->roffset <= returnCount) {
		copy_amount = file->rbufend - file->roffset;
		returnCount = copy_amount + buf_amount;
		if (count < BUF_MAX) {
			file->rbufend = -1;
		}
	}
	if (count < BUF_MAX) {
		memcpy((char *) buf + buf_amount, file->rbuf + file->roffset, copy_amount);
		file->roffset += count - buf_amount;
	}

	return returnCount;
}

ssize_t mywrite(myfile *file, const void *buf, size_t count) {
	ssize_t bytes_written;
	int buf_amount = 0;

	if (count + file->woffset <= BUF_MAX) {
		memcpy((char * ) file->wbuf + file->woffset, buf, count);
		//printf("%s\n",file->wbuf);
		if (count + file->woffset == BUF_MAX) {
			bytes_written = write(file->fd, file->wbuf, BUF_MAX);
		}

		file->woffset = count + file->woffset == BUF_MAX ? 0 : file->woffset + count;
		//printf("%d\n",file->woffset);
	// use valgrind
	// use write to 1 to print
		return count;
	}
	
	buf_amount = BUF_MAX - file->woffset;
	memcpy(file->wbuf + file->woffset, buf, buf_amount);
	bytes_written = write(file->fd, file->wbuf, BUF_MAX);
	file->woffset = 0;
	if (count - buf_amount >= BUF_MAX) {
		bytes_written = write(file->fd, (char *) buf + buf_amount, count - buf_amount);
	}
	else {
		memcpy(file->wbuf, (char *) buf + buf_amount, count - BUF_MAX);
		file->woffset += count - BUF_MAX;
	}

	return bytes_written;
}

int myclose(myfile *file) {
	if (file->woffset > 0) {
		write(file->fd, file->wbuf, file->woffset);
	}
	int success; //0 is success
	success = close(file->fd);
	free(file);
	return success;
}

int myseek(myfile *file, int offset, int whence) {
	int new_offset;
	new_offset = (int) lseek(file->fd, offset, whence);
	return new_offset;
}

int myflush(myfile *file) {
	int bytes_written;
	bytes_written = write(file->fd, file->wbuf, file->woffset);
	file->woffset = 0;
	return bytes_written;
}	
