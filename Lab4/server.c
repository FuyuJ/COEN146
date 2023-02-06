/*
 * Name: Marianne Fuyu Yamazaki Dorr
 * Date: Wednesday 5PM - 8PM
 * Title: Lab4 - Step 1
 * Description: This program creates a UDP server that receives a file from a client.
 */
//COEN 146L : Lab4, step 1: UDP server (peer) that receives a file from a client (peer)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

//Declare a Header structure that holds length of a packet
typedef struct {
    int len;
} Header;

//Declare a packet structure that holds data and header
typedef struct {
    Header header;
    char data[10];
} Packet;

//Declare client address to fill in address of sender
struct sockaddr_in servAddr, clienAddr;

//Printing received packet
void printPacket(Packet packet) {
    printf("Packet{ header: { len: %d }, data: \"", packet.header.len);
    fwrite(packet.data, (size_t)packet.header.len, 1, stdout);
    printf("\" }\n");
}

//Server sending ACK to the client
void serverSend(int sockfd, const struct sockaddr *address, socklen_t addrlen) {
    Packet packet;	
    //sending acknowledgement packet
    strcpy(packet.data, "Acknowledg");
    packet.header.len=sizeof(packet.data);
	
    //send packet
    sendto(sockfd, &packet, sizeof(packet), 0, address, addrlen);
    printf("\t Server sending %s \n", packet.data);
}

Packet serverReceive(int sockfd, struct sockaddr *address, socklen_t *addrlen) {
    Packet packet;
    while (1) {
        //recv packets from the client
	    recvfrom(sockfd, &packet, sizeof(packet), 0, address, addrlen);
        
        // print received packet
        printPacket(packet);
        
        //send acknowldgement
	    serverSend(sockfd, address, *addrlen);
        break;
    }
    return packet;
}


int main(int argc, char *argv[]) {
    //Get from the command line, server IP and dst file
    if (argc != 3) {
        printf("Usage: %s <port> <dstfile>\n", argv[0]);
        exit(0);
    }
    //Declare socket file descriptor.
    int sockfd; 

    //Open a UDP socket, if successful, returns a descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
	perror("Failure to setup an endpoint socket\n");
	exit(1);
	}

    //Setup the server address to bind using socket addressing structure
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(atoi(argv[1])); 
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);     

    //bind IP address and port for server endpoint socket
    if ((bind(sockfd, (struct sockaddr *)&servAddr, sizeof(struct sockaddr))) < 0) {
	perror("Failure to bind server address to the endpoint socket\n");
	exit(1);
	}

    //open file given by argv[2], create if it does not exist
    int fp = open(argv[2], O_CREAT | O_RDWR, 0666);
    if(fp < 0){
	    perror("File failed to open\n");
	    exit(1);
    }
    // get file contents (as packets) from client
    Packet packet;
    socklen_t addr_len = sizeof(clienAddr);
    printf("Waiting for packets to come.....\n");
    do {
        packet = serverReceive(sockfd, (struct sockaddr *)&clienAddr, &addr_len);
	    write(fp, packet.data, packet.header.len);
    } while (packet.header.len != 0);
    //close file and socket
	close(fp);
	close(sockfd);
    return 0;
}
