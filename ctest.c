
/*
 * ctest.c
 *
 * usage: ./ctest
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "myio.h"
#define BYTES_PER_ITERATION 10



int
main(int argc, char *argv[])
{
    myfile  *in1, *out1, *in2, *out2;
    char *src_filename1, *dst_filename1, *src_filename2, *dst_filename2;
    size_t bytes_read;
    char buf1[BYTES_PER_ITERATION];
    char buf2[BYTES_PER_ITERATION];

    src_filename1 = "files/ctest_1s.txt";
    dst_filename1 = "files/ctest_1d.txt";
    src_filename2 = "files/ctest_2s.txt";
    dst_filename2 = "files/ctest_2d.txt";

    /* open files */
    if((in1 = myopen(src_filename1, O_RDONLY | O_CREAT,00744))->fd == -1) {
        perror("open");
        exit(3);
    }
    if((out1 = myopen(dst_filename1, O_WRONLY | O_CREAT,00744))->fd == -1) {
        perror("open");
	printf("Hi\n");
        exit(3);
    }
    if((in2 = myopen(src_filename2, O_RDONLY | O_CREAT,00744))->fd == -1) {
        perror("open");
        exit(3);
    }
    if((out2 = myopen(dst_filename2, O_WRONLY | O_CREAT,00744))->fd == -1) {
        perror("open");
        exit(3);
    }
    
    /*   do the copy */ 
    bytes_read = myread(in1, buf1, BYTES_PER_ITERATION);
    mywrite(out1, buf1, bytes_read);
    bytes_read = myread(in2, buf2, BYTES_PER_ITERATION);
    mywrite(out2, buf2, bytes_read);
    
    bytes_read = myread(in1, buf1, BYTES_PER_ITERATION);
    mywrite(out1, buf1, bytes_read);
    bytes_read = myread(in2, buf2, BYTES_PER_ITERATION);
    mywrite(out2, buf2, bytes_read);

    bytes_read = myread(in1, buf1, BYTES_PER_ITERATION);
    mywrite(out1, buf1, bytes_read);
    bytes_read = myread(in2, buf2, BYTES_PER_ITERATION);
    mywrite(out2, buf2, bytes_read);
    
    bytes_read = myread(in1, buf1, BYTES_PER_ITERATION);
    mywrite(out1, buf1, bytes_read);
    bytes_read = myread(in2, buf2, BYTES_PER_ITERATION);
    mywrite(out2, buf2, bytes_read);
    
    /* clean up */
    if(myclose(out2) == -1) {
	perror("close");
	exit(6);
    }
    if(myclose(in2) == -1) {
	perror("close");
	exit(6);
    }
    if(myclose(out1) == -1) {
	perror("close");
	exit(6);
    }
    if(myclose(in1) == -1) {
	perror("close");
	exit(6);
    }
}
