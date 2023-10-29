#include "myio.h"




#define buf_max 10
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

	if(file->rbufend == -1) {
		return 0;
	}
	
	//check if call will empty buffer
	if (file->roffset + count == buf_max) {
		if (file->rbufend - file->roffset < returnCount) {
			returnCount = file->rbufend - file->roffset;
		}
		
		//we read less than we want, so file is empty, trigger end condition
		if(file->rbufend < buf_max) {
			file->rbufend = -1;
		}

		memcpy(buf,file->rbuf + file->roffset,returnCount);
		file->roffset=0;
		
		return returnCount;
	}
	
	//check if call will exceed buffer
	else if (file->rbufend && file->roffset + count > buf_max) {
		buf_amount = buf_max - file->roffset;
		memcpy(buf,file->rbuf + file->roffset,buf_amount);
		file->roffset = 0; 	
	}


	//either first call or we need more stuff
	if (file->roffset == 0) {
		int buf_bytes = buf_max;		
		if(count >= buf_max) {
			buf_bytes = count - buf_amount;
		}

		// we read bytes read into file->rbufend: need to know if the end of file is closer than buffer end 
		file->rbufend = read(file->fd, file->rbuf, buf_bytes);

		if (file->rbufend == 0) {
			return 0;
		}
	}

	// if what is left is less than what we asked for, trigger end condition	
	int copy_amount = count - buf_amount;
	if (file->rbufend - file->roffset <= returnCount) {
		copy_amount = file->rbufend - file->roffset;
		returnCount = copy_amount + buf_amount;
		file->rbufend = -1;
	}

	// with all of that being said, actually do the copying from buffer to buffer
	memcpy((char *) buf + buf_amount,file->rbuf + file->roffset,copy_amount);

	//advance beginning of buffer pointer
	file->roffset += count - buf_amount;

	return returnCount;
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


