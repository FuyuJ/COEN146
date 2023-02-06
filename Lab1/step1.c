/*
 # Name: Marianne Fuyu Yamazaki Dorr
 # Date: Wednesday 5:15-8:00 PM
 # Title: Lab1 - Basic Linux and C programming skills
 # Description: This program copies files (binary and text) using functions. 
 */
 
#include <pthread.h> // pthread_create()
#include <unistd.h>  // read(), write()
#include <fcntl.h>   // open(), close()
#include <errno.h>   // errno
#include <time.h>    // clock()
#include <sys/mman.h> // mmap()
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#define BUF_SIZE 2048 //buffer size

// copies a files from src_filename to dst_filename using functions fopen(), 
// fread(), fwrite(), fclose()
 int func_copy(char* src_filename, char* dst_filename) {
	FILE *src, *dst;
	src = fopen(src_filename, "rb"); // opens a file for reading
 	if (src == NULL) { // fopen() error checking
 		fprintf(stderr, "unable to open %s for reading: %i\n", src_filename, errno);
 		exit(0);
 	}
 	dst = fopen(dst_filename, "w"); // opens a file for writing; erases old file/creates a new file
 	if (dst == NULL) { // fopen() error checking
 		fprintf(stderr, "unable to open/create %s for writing: %i\n", dst_filename, errno);
 		exit(0);
 	}
	void* buf = malloc((size_t)BUF_SIZE); // allocate a buffer to store read data
 	// reads content of file is loop iterations until end of file
 	// writes bytes_read to dst_filename
	int bytes_read;
	while((bytes_read = fread(buf, 1,sizeof(buf), src)) > 0) {
		fwrite(buf, 1, bytes_read, dst);
	}
	// closes src file pointer
	fclose(src);
 	// closes dst file pointer
 	fclose(dst);
 	// frees memory used for buf
 	free(buf);
 	return 0;
}

 
int main(int argc, char* argv[]) {
	if (argc != 3) {  // check correct usage
 		fprintf(stderr, "usage: %s <src_filename> <dst_filename>\n", argv[0]);
 		exit(1);
 	}
 	//get the source and destination files from the command line arguments
	char *src = argv[1];
	char *dst = argv[2];	
	//call the check copy times function
	func_copy(src, dst);
 	return 0;
}
