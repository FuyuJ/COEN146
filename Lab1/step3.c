/*
 # Name: Marianne Fuyu Yamazaki Dorr
 # Date: Wednesday 5:15-8:00 PM
 # Title: Lab1 - Basic Linux and C programming skills
 # Description: This program copies files (binary and text) using functions and system 
 # 		calls. It will also display the time it takes to complete both.  
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
	void* buf = malloc((size_t)BUF_SIZE);
 	//buf = malloc((size_t)BUF_SIZE);  // allocate a buffer to store read data
 	// reads content of file is loop iterations until end of file
	int bytes_read;
	while((bytes_read = fread(buf, 1, sizeof(buf), src)) > 0) {
		fwrite(buf, 1, bytes_read, dst);
	}
 	// writes bytes_read to dst_filename 
	//did not use bytes_read, simply copied as it read
	// closes src file pointer
	fclose(src);
 	// closes dst file pointer
 	fclose(dst);
 	// frees memory used for buf
 	free(buf);
 	return 0;
 }
 
// copies a files form src_filename to dst_filename using syscalls open(), read(), write(), close()
 int syscall_copy(char* src_filename, char* dst_filename)  {
 	int src_fd = open(src_filename, O_RDONLY); // opens a file for reading
 	if (src_fd < 0) { // open() error checking
 		fprintf(stderr, "unable to open %s for reading: %i\n", src_filename, errno);
 		close(src_fd);
 		exit(0);
 	}	
	// O_WRONLY - write only
 	// O_CREAT - create file if it doesn't exist
 	// O_TRUNC - truncates file to length 0 if it already exists (deletes existing file)
 	// opens a file for writing; erases old file/creates a new file
 	int dst_fd = open(dst_filename, O_WRONLY | O_CREAT | O_TRUNC);
 	if (dst_fd < 0) { // open() error checking
 		fprintf(stderr, "unable to open/create %s for writing: %i\n", 
 		dst_filename, errno);
 		close(dst_fd);
 		exit(0);
 	}
 	

	// read/ write loop
 	char *buf;
	buf = malloc((size_t)BUF_SIZE);   // allocate a buffer to store read data
 	// reads up to BUF_SIZE bytes from src_filename
 	// writes bytes_read to dst_filename
 	int bytes_read;
	while((bytes_read = read(src_fd, buf, BUF_SIZE)) > 0) {
		write(dst_fd, buf, bytes_read);
	}	
	// closes src_fd file descriptor
	close(src_fd);
 	// closes dst_fd file descriptor
 	close(dst_fd);
 	// frees memory used for buf
 	free(buf);
	return 0;
 }
 
// checks the runtime to copy a file using functions vs syscalls
void check_copy_times(char* src_filename, char* dst_filename) {
 	clock_t func_start, func_end, syscall_start, syscall_end;
 	// capture runtime of func_copy() using start clock, call the copy, end the clock
 	clock_t start, end;
	double func_time;
	start = clock();
	func_copy(src_filename, dst_filename);
	end = clock();
	func_time = ((double) (end - start)) / CLOCKS_PER_SEC;	
	// capture runtime of syscall_copy() using start clock, call the copy, end the clock
	double syscall_time;
	start = clock();
	syscall_copy(src_filename, dst_filename);
	end = clock();
	syscall_time = ((double) (end - start)) / CLOCKS_PER_SEC;
	//print the results out
	printf("time to copy using functions: %.7f\n", func_time);
 	printf("time to copy using syscalls: %.7f\n", syscall_time);
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
	check_copy_times(src, dst);
 	return 0;
}
