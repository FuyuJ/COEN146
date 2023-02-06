/*
 * Name: Marianne Fuyu Yamazaki Dorr
 * Date: Wednesday 5PM - 8PM
 * Title: Lab 3 - Step 1
 * Description: A program for a TCP server that accepts a client connection for file transfer
 */

//COEN 146L: Lab3, step 1: TCP server that accepts a client connection for file transfer

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

//Declare socket file descriptor.
int sockfd, connfd, rb, sin_size, new_connfd;
//Declare receiving and sending buffers of size 10 bytes
char sbuf[10], rbuf[10];
//Declare server address to which to bind for receiving messages and client address to fill in sending address
struct sockaddr_in servAddr, clienAddr;

//Connection handler for servicing client request for file transfer
void* connectionHandler(void* sock) {
	//declare buffer holding the name of the file from client
	char filename[1024];	
	//get the connection descriptor
	new_connfd = *(int*)sock;
	
	//print statement below causes seg fault error if included
	//Connection established, server begins to read and write to the connecting client
	//	printf("Connection Established with client IP: %s and Port: %d\n",
	//		inet_ntoa(clienAddr.sin_addr), ntohs(clienAddr.sin_port));
	//receive name of the file from the client
	read(new_connfd, filename, sizeof(filename)); 	
	printf("%s \n",filename);	
	//open file and send to client
	FILE *fd;
	fd = fopen(filename, "r");
	//read file and send to connection descriptor
	while ((rb = fread(&rbuf, 1, sizeof(rbuf), fd)) > 0) {
		write(new_connfd, rbuf, rb);
	}		
	printf("File transfer complete\n");

	//close file
	fclose(fd);
	//close connection descriptor
	close(new_connfd);
	return 0;
}

int main(int argc, char *argv[]) {
	//Get from command line port #
	if (argc != 2){
		printf("usage: %s <port #> \n", argv[0]);
		exit(0);
	}
	//Open a TCP socket, if successful, returns a descriptor
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Failure to setup and endpoint socket.\n");
		exit(0);
	}	
	//Setup the server address to bind using socket addressing structure 
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(atoi(argv[1]));
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	 
	//bind IP address and port for server endpoint socket
	if ((bind(sockfd, (struct sockaddr *)&servAddr, sizeof(struct sockaddr))) <0) {
		perror("Failure to bind server address to endpoint socket\n");
		exit(1);
	}
	
	//Server listening to the socket endpoint, and can queue 5 client requests
	printf("Server listening/waiting for client at port 5000\n");
	listen(sockfd, 5);
	
	sin_size = sizeof(struct sockaddr_in);
	
	//Server accepts the connection and call the connection handler 
	if((connfd = accept(sockfd, (struct sockaddr *) &clienAddr, (socklen_t *)&sin_size)) < 0) {
		perror("Failure to accept connection to the client.\n");
		exit(1);
	}
	connectionHandler((void *) &connfd);
	
	//close socket descriptor
	close(sockfd);
	return 0;
	}
		
