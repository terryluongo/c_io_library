#include "myio.h"

#include <unistd.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>


#define BUF_MAX 4096 
#define FILE_END -1
#define FILE_UNINIT -2

// keep track of bytes asked for write and read
// for each do lseek reversing and then redoing the opposite bytes asked
// lseek local mode is absolute with total bytes + requested amount 
myfile *myopen(const char *pathname, int flags,mode_t mode) {
	int fd;
	if ((fd = open(pathname, flags, mode)) == -1) {
		return(NULL);
	}
	myfile *file;
	if ((file = malloc(sizeof *file)) != NULL) {
		file->fd = fd;
		file->roffset = 0;
		file->woffset = 0;
		file->rbufend = FILE_UNINIT;
		file->total_read = 0;
		file->total_write = 0;
		file->last_write = 0;
	}
	return file;
}

ssize_t myread(myfile *file, void *buf, size_t count) {
	size_t returnCount = count;
	int buf_amount = 0;
	//printf("--------------------------");
	// only needs to have myseek modified when actual syscall is taking place 
	// act on sentinel value from before and return
	if (file->rbufend == FILE_END) {
		return 0;
	}
	// flush out write buffer; bytes need to be delievered.
	if(file->last_write) myflush(file);
	file->last_write = 0;

	//check if call will empty buffer
	if ((file->rbufend != FILE_UNINIT) && (file->roffset + count == BUF_MAX)) {
		if (file->rbufend - file->roffset < returnCount) {
			returnCount = file->rbufend - file->roffset;
		}
		
		//our syscall read less than the BUF_MAX, so file is empty, trigger end condition
		if (file->rbufend < BUF_MAX) {
			file->rbufend = FILE_END;
		}
		memcpy(buf, file->rbuf + file->roffset, returnCount);
		file->roffset=0;
		
		file->total_read += returnCount;
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
			// copy buf_amount last bytes in buffer to return buffer
			memcpy(buf, file->rbuf + BUF_MAX - buf_amount, buf_amount);
			file->total_read += buf_amount;
			return buf_amount;
		}
	}

	// if what is left is less than what we asked for, trigger end condition	
	int copy_amount = count - buf_amount;

	if (file->rbufend - file->roffset <= returnCount) {
		copy_amount = file->rbufend - file->roffset;
		returnCount = copy_amount + buf_amount;

		if (count < BUF_MAX) {
			file->rbufend = FILE_END;
		}
	}
	if (count < BUF_MAX) {
		memcpy((char *) buf + buf_amount, file->rbuf + file->roffset, copy_amount);
		file->roffset += copy_amount;
	}

	file->total_read += returnCount;
	return returnCount;
}

ssize_t mywrite(myfile *file, const void *buf, size_t count) {
	// so if we wrote for amount smaller than BUF_MAX, we have stuff in buffer
	// then we ask for more than buffer
	// empty buffer to write 
	// then write remaining bytes straight to file 
	ssize_t bytes_written;
	int buf_amount = 0;

	if(!(file->last_write)) myseek(file, 0, SEEK_CUR);

	file->last_write = 1;

	//myseek(file, -file->total_read, SEEK_CUR);

	file->total_write += count;

	if (count + file->woffset <= BUF_MAX) {
		memcpy((char * ) file->wbuf + file->woffset, buf, count);

		if (count + file->woffset == BUF_MAX) {
			bytes_written = write(file->fd, file->wbuf, BUF_MAX);
		}

		file->woffset = count + file->woffset == BUF_MAX ? 0 : file->woffset + count;

		//myseek(file, file->total_read, SEEK_CUR);
		return count;
	}
	
	buf_amount = BUF_MAX - file->woffset;

	memcpy(file->wbuf + file->woffset, buf, buf_amount);

	bytes_written = write(file->fd, file->wbuf, BUF_MAX);

	file->woffset = 0;

	if (count - buf_amount >= BUF_MAX) {
		bytes_written = write(file->fd, (char *) buf + buf_amount, count - buf_amount) + buf_amount;
	}
	else {
		memcpy(file->wbuf, (char *) buf + buf_amount, count - buf_amount);
		file->woffset += count - buf_amount;
	}

	//myseek(file, file->total_read, SEEK_CUR);
	return bytes_written;
}

int myclose(myfile *file) {
	write(file->fd, file->wbuf, file->woffset);

	int success; //0 is success
	success = close(file->fd);

	free(file);

	return success;
}

int myseek(myfile *file, int offset, int whence) {
	int new_offset;

	int total_bytes = file->total_read + file->total_write;
	file->roffset = 0; //so that status of myread reflects status of myseek	
	myflush(file);	

	int absol_offset = (whence == SEEK_SET) ? offset : total_bytes + offset;
	new_offset = (int) lseek(file->fd, absol_offset, SEEK_SET);
	return new_offset;
}

int myflush(myfile *file) {
	int bytes_written;
	bytes_written = write(file->fd, file->wbuf, file->woffset);
	file->woffset = 0;
	return bytes_written;
}	
