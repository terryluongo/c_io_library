#include <unistd.h>

int myopen(const char *pathname, int flags);
ssize_t myread(int fd, void buf[.count], size_t count);
ssize_t mywrite(int fd, const void buf[.count], size_t count);
int myclose(int fd);


