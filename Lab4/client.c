/*
 * Name: Marianne Fuyu Yamazaki Dorr
 * Date: Wednesday 5PM - 8PM
 * Title: Lab4 - Step 2
 * Description: This program creates a UDP client that sends a file to a server.
 */ 
//COEN 146L : Lab4, step 2: UDP client (peer) that sends a file to a server (peer)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
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

//Printing received packet
void printPacket(Packet packet) {
    printf("Packet{ header: { len: %d }, data: \"",
            packet.header.len);
    fwrite(packet.data, (size_t)packet.header.len, 1, stdout);
    printf("\" }\n");
}

void clientSend(int sockfd, const struct sockaddr *address, socklen_t addrlen, Packet packet) {
    while (1) {	
        //send the packet
        printf("Client sending packet\n");
        sendto(sockfd, &packet, sizeof(packet), 0, address, addrlen);  
        //receive an ACK from the server
        Packet recvpacket;
        recvfrom(sockfd, &recvpacket, sizeof(recvpacket), 0, (struct sockaddr *)address, &addrlen);
        //print received packet
        printPacket(recvpacket);
        break;
    }
}

int main(int argc, char *argv[]) {
    //Get from the command line, server IP, Port and src file
    if (argc != 4) {
        printf("Usage: %s <ip> <port> <srcfile>\n", argv[0]);
        exit(0);
    }
    
    //Declare socket file descriptor.
    int sockfd; 

    //Open a UDP socket, if successful, returns a descriptor    
	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("Failure to setup an endpoint socket\n");
		exit(1);
	}    
    //Declare server address and get host to connect to
    struct sockaddr_in servAddr;
    struct hostent *host;
    host = (struct hostent *) gethostbyname(argv[1]);

    //clear memory segment holding servAddr
    memset(&servAddr, 0, sizeof(servAddr));

    //Set the server address to send using socket addressing structure 
    servAddr.sin_port = htons(atoi(argv[2]));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr= *((struct in_addr *)host->h_addr);

    //Open file given by argv[3]
    int fp=open(argv[3], O_RDWR);
    if(fp < 0){
    	perror("Failed to open file\n");
	    exit(1);
    }

    // send file contents to server
    socklen_t addr_len = sizeof(servAddr);
    Packet packet;
    int rb;
    while((rb = read(fp, packet.data, sizeof(packet.data))) > 0){
    	packet.header.len=rb;
    	clientSend(sockfd, (struct sockaddr *)&servAddr, addr_len, packet);
    }
    //sending zero-length packet (final) to server to end connection
    Packet final;
    final.header.len=0;
    clientSend(sockfd,(struct sockaddr *)&servAddr, addr_len, final);

    //close file and socket
    close(fp);
    close(sockfd);
    return 0;
}
