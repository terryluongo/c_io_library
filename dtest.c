
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
    myfile  *in1, *out1;
    char *src_filename1, *dst_filename1;
    size_t bytes_read;
    char buf1[BYTES_PER_ITERATION];

    src_filename1 = "files/ctest_1s.txt";
    dst_filename1 = "files/ctest_1d.txt";

    /* open files */
    if((in1 = myopen(src_filename1, O_RDONLY,00744))->fd == -1) {
        perror("open");
        exit(3);
    }
    if((out1 = myopen(dst_filename1, O_WRONLY | O_CREAT,00744))->fd == -1) {
        perror("open");
	printf("Hi\n");
        exit(3);
    }
	myseek(in1,0,SEEK_SET);    
    /*   do the copy */ 
    bytes_read = read(in1->fd, buf1, BYTES_PER_ITERATION);
    mywrite(out1, buf1, bytes_read);
   	printf("%s\n",buf1); 
	printf("%ld\n",bytes_read);
    bytes_read = myread(in1, buf1, BYTES_PER_ITERATION);
    mywrite(out1, buf1, bytes_read);

    bytes_read = myread(in1, buf1, BYTES_PER_ITERATION);
    mywrite(out1, buf1, bytes_read);
    bytes_read = myread(in1, buf1, BYTES_PER_ITERATION);
    mywrite(out1, buf1, bytes_read);
    
    /* clean up */
    if(myclose(out1) == -1) {
	perror("close");
	exit(6);
    }
    if(myclose(in1) == -1) {
	perror("close");
	exit(6);
    }
}
