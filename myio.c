#include "myio.h"



#ifndef buf_max
#define buf_max 40 
#endif


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
	size_t returnCount = count;
	int buf_amount = 0;

	// act on sentinel value from before and return
	if(file->rbufend == -1) {
		return 0;
	}
		
	//check if call will empty buffer
	if (file->roffset + count == buf_max) {
		if (file->rbufend - file->roffset < returnCount) {
			returnCount = file->rbufend - file->roffset;
		}
		
		//our syscall read less than the buf_max, so file is empty, trigger end condition
		if(file->rbufend < buf_max) {
			file->rbufend = -1;
		}
		memcpy(buf,file->rbuf + file->roffset,returnCount);
		file->roffset=0;
		
		return returnCount;
	}
	
	//check if call will exceed buffer
	else if (file->rbufend && file->roffset > 0 && file->roffset + count > buf_max) {
		buf_amount = buf_max - file->roffset;
		memcpy(buf,file->rbuf + file->roffset,buf_amount);
		file->roffset = 0; 	
	}

	//either first call or we need more stuff
	if (file->roffset == 0) {

		if(count >= buf_max) {
			file->rbufend = read(file->fd, (char *) buf + buf_amount, count-buf_amount);
		}

		else {
		file->rbufend = read(file->fd, file->rbuf, buf_max);
		}
		if (file->rbufend == 0) {
			memcpy(buf,file->rbuf + buf_max - buf_amount,buf_amount);
			return buf_amount;
		}
	}

	// if what is left is less than what we asked for, trigger end condition	
	int copy_amount = count - buf_amount;
	if (file->rbufend - file->roffset <= returnCount) {
		copy_amount = file->rbufend - file->roffset;
		returnCount = copy_amount + buf_amount;
		if(count < buf_max) {
			file->rbufend = -1;
		}
	}
	if (count < buf_max) {
		memcpy((char *) buf + buf_amount, file->rbuf + file->roffset, copy_amount);
		file->roffset += count - buf_amount;
	}

	return returnCount;
}
ssize_t mywrite(myfile *file, const void *buf, size_t count) {
	ssize_t bytes_written;
	int buf_amount = 0;

	if (count + file->woffset <= buf_max) {
		memcpy((char * ) file->wbuf + file->woffset, buf, count);
		//printf("%s\n",file->wbuf);
		if (count + file->woffset == buf_max) {
			bytes_written = write(file->fd,file->wbuf,buf_max);
		}

		file->woffset = count + file->woffset == buf_max ? 0 : file->woffset + count;
		//printf("%d\n",file->woffset);
	// use valgrind
	// use write to 1 to print
		return count;
	}
	
	buf_amount = buf_max - file->woffset;
	memcpy(file->wbuf + file->woffset, buf, buf_amount);
	bytes_written = write(file->fd,file->wbuf,buf_max);
	file->woffset = 0;
	if (count - buf_amount >= buf_max) {
	bytes_written = write(file->fd, (char *) buf + buf_amount, count - buf_amount);
	}
	else {
		memcpy(file->wbuf, (char *) buf + buf_amount, count - buf_max);
		file->woffset += count - buf_max;
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
	new_offset = (int) lseek(file->fd,offset,whence);
	return new_offset;
}



int myflush(myfile *file) {
	int bytes_written;
	bytes_written = write(file->fd,file->wbuf,file->woffset);
	file->woffset = 0;
	return bytes_written;
}	
