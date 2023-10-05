/*
 * copy-file.c
 *
 * usage: ./copy-file source-file destination-file
 */

#include <stdio.h>
#include <stdlib.h>

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



*/
int
main(int argc, char *argv[])
{
    FILE *in, *out;
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
    in = fopen(src_filename, "r");
    if(in == NULL) {
        perror("fopen");
        exit(2);
    }

    if((out = fopen(dst_filename, "w")) == NULL) {
        perror("fopen");
        exit(3);
    }

    /* do the copy */
    while((bytes_read = fread(buf, 1, BYTES_PER_ITERATION, in)) > 0) {
        bytes_written = fwrite(buf, 1, bytes_read, out);
        if(bytes_written == 0) {
            perror("fwrite");
            exit(4);
        }
    }

    /* the while loop above could terminate if fread encountered an error, so
     * check that and report if necessary */
    if(ferror(in)) {
        perror("fread");
        exit(5);
    }

    /* clean up */
    fclose(out);
    fclose(in);
}

