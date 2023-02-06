/*
 * Name: Marianne Yamazaki Dorr
 * Date: Wednesday 5pm - 8pm
 * Title: Lab2 - Network Commands, Packet Delays, and HTTP
 * Description: This program calculate the amount of time in msecs that it takes for a client to 
 * 		receive their object. It also computes the response time in both persistent and 
 * 		non-persistent states.
 */
//COEN 146L: Lab2, step3
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int main(int argc, char *argv[]) {
	if (argc != 2) { //check correct usage
		fprintf(stderr, "usage: %s <n connections> \n", argv[0]);
		exit(1);
	}
	//Declare and values to n (n connections, np (np parralel connections), RTT0, RTT1, RTT2, RTTHTTP, RTTDNS, ...
	int RTT0 = 3;
	int RTT1 = 20;
	int RTT2 = 26;
	int RTTHTTP = 47;
	int objects = 6;
	//set number of connections as giving by command line argument
	int connections  = atoi(argv[1]);
	//calculate transmission time for one object
	int transmission = RTT0 + RTT1 + RTT2 + (2*RTTHTTP); 	
	printf("One object: %d msec\n", transmission);
	//calculate transmission time for 6 objects
	int sixTransmission = transmission + (objects * 2 * RTTHTTP);
	printf("Non-Persistent 6 objects: %d msec\n", sixTransmission);
	
	//find how many np (parallel connections)
	int np = ceil((float)objects/(float)connections);
	//non-persistent connection response time
	int nonPersistent = transmission + (2 *ceil((float)objects/(float)connections)) * RTTHTTP;
	//persistent connection response time
	int persistent = transmission + (ceil((float)objects/(float)connections)) * RTTHTTP;
	
	printf("%d parallel connection - Persistent: %d msec\n", np, persistent);
	printf("%d parallel connection - Non-Persistent: %d msec\n", np, nonPersistent);

	return 0;
}
