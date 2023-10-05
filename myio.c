int myopen(const char *pathname, int flags) {
	int fd;	
	fd = open(pathname,flags);
	return fd;
}
ssize_t myread(int fd, void buf[.count], size_t count) {
	ssize_t amountRead;	
	amountRead = read(fd, buf, count);
	return amountRead;
}
ssize_t mywrite(int fd, const void buf[.count], size_t count) {
	ssize_t bytes_written;
	bytes_written = write(fd, buf, count);
	return bytes_written;
}
int myclose(int fd) {
	int success;
	success = close(fd);
	return success;
}


