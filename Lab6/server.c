//COEN 146L: Lab6, UDP server with packets that have checksum, sequence number, acknowledgement number, and timer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>

//Declare a Header structure that holds, sequence/ acknowledgement number, checksum, and length of a packet
typedef struct {
    int seq_ack;
    int len;
    int cksum;
} Header;

//Declare a packet structure that holds data and header
typedef struct {
    Header header;
    char data[10];
} Packet;

//Declare client address to fill in address of sender
struct sockaddr_in servAddr, clienAddr;

//Calculate the Checksum
int getChecksum(Packet packet) {
    packet.header.cksum = 0;
    int checksum = 0;
    char *ptr = (char *)&packet;
    char *end = ptr + sizeof(Header) + packet.header.len;
    while (ptr < end) {
        checksum ^= *ptr++;
    }

    return checksum;
}

//Print received packet: data, sequence/acknowledgement number, and checksum
void printPacket(Packet packet) {
    printf("Packet{ header: { seq_ack: %d, len: %d, cksum: %d }, data: \"",
            packet.header.seq_ack,
            packet.header.len,
            packet.header.cksum);
    fwrite(packet.data, (size_t)packet.header.len, 1, stdout);
    printf("\" }\n");
}

//Server sending ACK to the client
void serverSend(int sockfd, const struct sockaddr *address, socklen_t addrlen, int seqnum) {
    //simulating a chance that ACK gets lost
    if (rand() % 5 == 0){ // this give a probability of 20% for a loss of ACK 
    	printf("Dropping ACK\n");
    }
    else{
        Packet packet;	
        packet.header.seq_ack = seqnum;
        packet.header.len = sizeof(packet.data);
        packet.header.cksum = getChecksum(packet);
        //send packet
        sendto(sockfd, &packet, sizeof(packet), 0, address, addrlen); 

        printf("\t Server sending ACK %d, checksum %d\n", packet.header.seq_ack, packet.header.cksum);
    }
}

//Server receiving Packet from the client
Packet serverReceive(int sockfd, struct sockaddr *address, socklen_t *addrlen, int seqnum) {
    Packet packet;
    while (1) {
        //recv packets from the client
        recvfrom(sockfd, &packet, sizeof(packet), 0, address, addrlen);	     
        
        // print received packet
        printPacket(packet);
	    
        //cacluate the checksum
        int e_cksum;
        e_cksum = getChecksum(packet);

        //check the checksum
        if (packet.header.cksum!=e_cksum) {
            printf("\t Server: Bad checksum, expected checksum was: %d\n", e_cksum);
            serverSend(sockfd, address, *addrlen, !seqnum);
        
        //check the sequence number
        } else if (packet.header.seq_ack != seqnum) {
            printf("\t Server: Bad seqnum, expected sequence number was:%d\n",seqnum);
            //resend packet
            serverSend(sockfd,address,*addrlen,!seqnum);
        
        //if the ckechsum and sequence numbers are correct send ack with the right sequence number
        } else {
            printf("\t Server: Good packet\n");

            //send ack
	    serverSend(sockfd, address, *addrlen, seqnum); 
            break;
        }
    }
    return packet;
}

int main(int argc, char *argv[]) {
    
    //Get from the command line, server IP and dst file
    if (argc != 3) {
        printf("Usage: %s <port> <dstfile>\n", argv[0]);
        exit(0);
    }

    //Declare socket file descriptor
    int sockfd;
    
    //Open a UDP socket, if successful, returns a descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Failure to setup endpoint socket\n");
	exit(1);
    }    

    //Setup the server address to bind using socket addressing structure
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(atoi(argv[1]));
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    //bind IP address and port for server endpoint socket
    if ((bind(sockfd, (struct sockaddr *)&servAddr, sizeof(struct sockaddr))) <0) {
	perror("Failure to bind server address to the endpoint socket\n");
	exit(1);
    }

    //open file using argv[2]
    int fp = open(argv[2], O_CREAT | O_RDWR, 0666);
    if (fp <0) {
	perror("File failed to open \n");
	exit(1);
    }
    //initialize sequence number to 0
    int seqnum = 0;
    
    //get file contents (as packets) from client
    Packet packet;
    socklen_t addr_len = sizeof(clienAddr);
    printf("Waiting for packets to come.....\n");
    do {
        //receive a packet and check through serverReceive()
     	packet = serverReceive(sockfd, (struct sockaddr *)&clienAddr, &addr_len, seqnum);   
	    write(fp, packet.data, packet.header.len);
        //alternate the sequence number between 0 and 1
        seqnum = (seqnum+1) % 2;
    } while (packet.header.len != 0);

    //close file and socket descriptors
    close(fp);
    close(sockfd);
    return 0;
}
