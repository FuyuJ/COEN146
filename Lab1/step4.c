/*
 * Name: Marianne Fuyu Yamazaki Dorr
 * Date: 01/14/2022
 * Title: Lab1 - Basic Linux and C programming skills
 * Description: This program copies a source file to multiple destinations using thread.
 */
//COEN 146L : Lab 1 - template to use for creating multiple thread to make file transfers (step 4)

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <string.h>
#define BUF_SIZE 2048 //buffer size

//data structure to hold copy
struct copy_struct {
	int thread_id;
	char* src_filename;
	char* dst_filename;
};

//copies a files from src_filename to dst_filename using functions fopen(), fread(), fwrite(), fclose()
int func_copy(char* src_filename, char* dst_filename) {
	FILE *src, *dst;
	src = fopen(src_filename, "r"); //opens a file for reading
	if (src == NULL) {// fopen() error checking
		fprintf(stderr, "unable to open %s for reading: %i\n", src_filename, errno);
		exit(0);
	}

	dst = fopen(dst_filename, "w"); //opens a file for writing; erases old file/creates a new file
	if (dst == NULL) { //fopen() error checking
		fprintf(stderr, "unable to open/create %s for writing : %i\n", dst_filename, errno);
		exit(0);
	}
	
	void* buf = malloc((size_t)BUF_SIZE); //allocate a buffer to store read data
	//reads content of file is loop iterations until end of file
	// writes bytes_read to destination file name
	int bytes_read;
	while((bytes_read = fread(buf, 1, sizeof(buf), src)) > 0 ) {
		fwrite(buf, 1, bytes_read, dst);
	}	
	//closes src file pointer
	fclose(src);
	//closes dst file pointer
	fclose(dst);
	//frees memory used for buf
	free(buf);
	return 0;
}

//thread function to copy one file
void *copy_thread(void* arg) {
	struct copy_struct params = *(struct copy_struct*)arg; //cast/dereference 
	printf("thread[%i] - copying %s to %s\n", params.thread_id, params.src_filename, params.dst_filename);
	//call file copy function
	func_copy(params.src_filename, params.dst_filename);	
	pthread_exit(NULL);
}

int main (int argc, char* argv[]) {
	//check correct usage of arguments in comand line
	if (argc != 4) { 
		fprintf(stderr, "usage: %s <src_filename> <dst_filename> <dst_filename2>\n", argv[0]);
		exit(1);
	}	
	char* src_filename = argv[1];
	char* dst_filename = argv[2];
	char* dst_filenames[] = {argv[2], argv[3]}; //array of destination files
		
	int num_threads = 2; // number of threads to create
	
	pthread_t threads[num_threads]; //initialize threads
	struct copy_struct thread_params[num_threads]; //structure for each thread 
	int i;
	for (i = 0; i < num_threads; i++) {
		//initialize thread parameters
		thread_params[i].thread_id = i;
		thread_params[i].src_filename = src_filename;
		thread_params[i].dst_filename = dst_filenames[i];  
		//create each copy thread
		//use pthread_create(....);
		pthread_create(&threads[i], NULL, copy_thread, &thread_params[i]);
	}
	
	//wait for all threads to finsh
	for (i = 0; i < num_threads; i++) {
		pthread_join(threads[i], NULL);
	}
	pthread_exit(NULL);
}
