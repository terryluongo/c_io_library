
/*
 * copy-file.c
 *
 * usage: ./copy-file source-file destination-file
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "myio.h"
#define BYTES_PER_ITERATION 10240
/*
opendir(3)  - open directory
readdir(3) - read directory
closedir(3) - close directory
getopt(3) - get option character from command line argument list
stat(2) -  The stat() function obtains information about the file pointed to by path | probably for the -l flag
strftime(3) - format date and time
getpwnam(3) - something with passwords
localtime(3) - The functions ctime(), gmtime(), and localtime() all take as an argument a time value representing the time in seconds since the Epoch (00:00:00 UTC, January 1, 1970; see time(3)).
     When encountering an error, these functions return NULL and set errno to an appropriate value.


1. pick directory we want
2. opendir
3. read contents pointer by pointer
4. use stat to get info
5. repeat until error or done
6. close directory

later:
use strftime, localtime, getpwnam for -l flag

p

*/
int
main(int argc, char *argv[])
{
    int in, out;
    char *src_filename, *dst_filename;
    size_t bytes_read, bytes_written;
    char buf[BYTES_PER_ITERATION];

    /* check command-line arguments */
    if(argc != 3) {
        printf("usage: %s source dest\n", argv[0]);
        exit(1);
    }
    src_filename = argv[1];
    dst_filename = argv[2];

    /* open files */
    in = myopen(src_filename, O_RDONLY,0744); //changed to open()
    if(in == -1) {
        perror("open");
	printf("%d\n",in);
        exit(2);
    }
    printf("%d\n",in);
    if((out = myopen( dst_filename, O_WRONLY | O_CREAT,00744)) == -1) { //changed to open()
        perror("open");
	printf("%d\n",out);
        exit(3);
    }
    
  /*   do the copy */ 
    while((bytes_read = myread(in, buf, BYTES_PER_ITERATION)) > 0) { //changed to read()
        bytes_written = mywrite(out, buf, bytes_read); //changed to write()
        if(bytes_written == 0) {
            perror("write");
            exit(4);
        }
    }

    /* the while loop above could terminate if fread encountered an error, so
     * check that and report if necessary
    if(ferror(in)) {
        perror("read");
        exit(5);
    }
    */

    /* clean up */
    if(myclose(out) == -1) {
	perror("close");
	exit(6);
    }
    if(myclose(in) == -1) {
	perror("close");
	exit(6);
    }
}
