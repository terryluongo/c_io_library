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
	

	//tracking the start of each call, buf_max, how close it is to it
	//printf("myread called\n");
	//printf("roffset + count %lu\n",file->roffset + count);
	//printf("%d\n",buf_max);
	//file->rbufend = buf_max;	
	size_t returnCount = count;
	int buf_amount = 0;

	if(file->rbufend == -1) {
		return 0;
	}
	// if we call for whatever the specified buffer is 
	if(count >= buf_max) {
		file->rbufend = read(file->fd, buf, count);
		return file->rbufend;
	}
	//first thing we have to check is if this call will go over the buf_max
	//we have to check this before the simple case of a first read because we may have to call read to get more
	if (file->roffset + count == buf_max) {
		buf_amount = count;
		memcpy(buf,file->rbuf + file->roffset,count);
		file->roffset=0;
		if(file->rbufend < buf_max) {

			file->rbufend = -1;
		}
		return returnCount;
	}
	else if (file->rbufend && file->roffset + count > buf_max) {
		//printf("trigger flush\n");
		if(file->rbufend < buf_max) {
			printf("eof\n");
			printf("%d\n",file->roffset);
			printf("%d\n",file->rbufend);
			memcpy(buf,file->rbuf + file->roffset, file->rbufend);
			returnCount = file->rbufend - file->roffset;
			file->rbufend = -1;
			printf("returnCount: %ld\n",returnCount);
			return returnCount;
		}
		buf_amount = buf_max - file->roffset;
		printf("%d\n",buf_amount);
		// copy whatever is left into the buffer before restarting 
		//printf("%s\n",file->rbuf+file->roffset);
		memcpy(buf,file->rbuf + file->roffset,buf_amount);
		file->roffset = 0; 	
	}
	//either first call or we need more stuff
	if (file->roffset == 0) {
		// we read bytes read into file->rbufend: need to know if the end of file is closer than buffer end 
		file->rbufend = read(file->fd, file->rbuf, buf_max);
		if (file->rbufend == 0) {
			return 0;
		}
	}
	
	//  print what is left in the buffer
	//printf("%s\n",file->rbuf + file->roffset);	
	int amount = count - buf_amount;
	if (file->rbufend - file->roffset < returnCount) {
		amount = file->rbufend - file->roffset;
		returnCount = amount + buf_amount;
		file->rbufend = -1;
	}
	// HAVE TO DO SIMILAR THING BUT IF BUFFER IS PERFECT OVERLAP	
	//copy into myread buffer the properly advanced amount from read buf
	memcpy((char *) buf + buf_amount,file->rbuf + file->roffset,amount);
	//advance beginning of buffer pointer
	file->roffset += count - buf_amount;

	//end case
	if(file->rbufend == 0) {
		returnCount = 0;
	}
	//right before end case
	/*else if(file->rbufend < returnCount) {
		returnCount = file->rbufend;
	} */
	//retreat end of buffer pointer
	//file->rbufend -= returnCount;
	//printf("buf_amount: %d ",buf_amount);
	//printf("roffset: %d ",file->roffset);
	//printf("rbufend: %d ",file->rbufend);
	//printf("returnCount: %d\n",returnCount);
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


