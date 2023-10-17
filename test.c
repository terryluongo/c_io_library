
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





int
main(int argc, char *argv[])
{
    myfile  *in, *out;
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
    if(in->fd == -1) {
        perror("open");

        exit(2);
    }
    
    if((out = myopen( dst_filename, O_WRONLY | O_CREAT,00744))->fd == -1) { //changed to open()
        perror("open");
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
