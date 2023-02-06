/*
 * Name: Marianne Fuyu Yamazaki Dorr
 * Date: Wednesday 5PM - 8PM
 * Title: Lab 3 - Step 2
 * Description: This program is for a TCP client that connects to the server
 */

//COEN 146L : Lab3, step 2: TCP client requesting a file transfer from the server. 
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
	//get from the command line, server IP, src and dst files.
	if (argc != 5) {
		printf("Usage: %s <ip of server> <port #> <src_file> <dst_file>\n", argv[0]);
		exit(0);
	}
	//Declare socket file descriptor and buffer
	int sockfd;
	char rbuf[10], sbuf[10];
	
	//Declare server address to accept
	struct sockaddr_in servAddr;	
	//Declare host 
	struct hostent *host;
	//get hostname 
	host = (struct hostent *)gethostbyname(argv[1]);
	//Open a socket, if successful, returns
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Failure to setup endpoint socket\n");
		exit(0);
	}
	//Set the server address to send using socket addressing structure
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(atoi(argv[2]));
	servAddr.sin_addr = *((struct in_addr *)host->h_addr);
	//Connect to the server
	if (connect(sockfd, (struct sockaddr *)&servAddr, sizeof(struct sockaddr))){
		perror("Failed to connect to server\n");
		exit(1);
	}
	//Client sends the name of the file to retrieve from the server
	int bytes_read;
	write(sockfd, argv[3], strlen(argv[3])+ 1);
	//Client beings to write and read from the server
	FILE* dst;
	dst = fopen(argv[4], "w");
	if (dst == NULL) {
		fprintf(stderr, "unable to open/create %s for writing: %i\n", argv[4], errno);
	}
	while((bytes_read = read(sockfd, rbuf, sizeof(rbuf)))> 0) {
		fwrite(&rbuf, 1, bytes_read, dst);	
	}
	//Close socket descriptor
	close(sockfd);
	
	return 0;
}
