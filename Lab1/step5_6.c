/*
 * Name: Marianne Fuyu Yamazaki Dorr
 * Date: Wednesday 5:15pm - 8pm
 * Title: Lab 1 - Basic Linux and C programming skills
 * Description: This program implements quantitative comparisons between circuit switching and
 * 		packet switching with the following parameters:
 * 			bandwidth of a network link = int linkBandwidth
 * 			bandwidth required for given user = int userBandwidth
 * 			number of circuit switching users = int nCSusers
 * 			number of packet switching users = int nPSusers
 * 			% of time a packet switching user needs to transmit = double tPSuser
 * 			probability that a given (specific) packet switching user is busy
 * 				transmitting = double pPSusersBusy
 * 			probability that one (specific) packet switching user is not busy
 * 				transmitting = double nPSusersNotBusy
 */
/* COEN 146L : Lab1 - Step 5&6*/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

double factorial (int n) {
	//factorial function
	if (n == 0)
		return 1;
	return n*factorial(n-1);
}

double binomial(int x, int y) {
	double result = (factorial(x)/(factorial(y)*factorial(x-y)));
	return result;
}

int main(int argc, char *argv[]){
	if (argc !=5) {
		fprintf(stderr, "usage: ./a.out <linkBandwith> <userBandwidth> <tPSusers> ,npSusers. \n");
		exit(0);
	}
	int i, linkBandwidth, userBandwidth, nCSusers, nPSusers;
	double pPSusers, tPSusers, pPSusersBusy, pPSusersNotBusy;

	//Get values from command line 
	linkBandwidth = atoi(argv[1]);
	userBandwidth = atoi(argv[2]);
	tPSusers = atof(argv[3]);
	nPSusers = atoi(argv[4]);

	//5a: Circuit Switching Scenario
	//how many users --> nCSusers
	nCSusers = linkBandwidth/userBandwidth;
	printf("5a: Circuit switching scenario: %d users \n", nCSusers);

	//6: Packet Switching Scenario
	//compute: 6a, b, c, d, e, f, g, h
	pPSusers = tPSusers;
	pPSusersNotBusy = 1 - pPSusers;
	double pAllNotBusy = pow(pPSusersNotBusy,( nPSusers - 1));
	double pOneBusy = pPSusers * pow(pPSusersNotBusy,( nPSusers - 1)); 
	double pExactlyOne = nPSusers * pPSusers * pow(pPSusersNotBusy,( nPSusers - 1));
	double pTenBusy = pow(pPSusers, 10) * pow(pPSusersNotBusy,( nPSusers - 10));
	double userCombination =  factorial(nPSusers)/(factorial(10)*factorial(nPSusers - 10));
	double pAnyTenBusy = userCombination * pTenBusy;
	
	double result = 0;
	for (i = 11; i < nPSusers; i++) {
		result += binomial(nPSusers, i) * pow(pPSusers, i)* pow(pPSusersNotBusy, (nPSusers-i));
	}
	double pTenMoreBusy = result;
 	
	printf("6: Packet switching scenario\n");
	printf("6a: The probability that a given (specific) user is busy transmitting = %f\n", pPSusers);
	printf("6b: The probability that one specific other user is not busy = %f\n", pPSusersNotBusy);
	printf("6c: The probability that all of the other specific users are not busy = %e\n", pAllNotBusy);
	printf("6d: The probability that one specific user is transmitting and the remaining users are not transmitting = %e\n", pOneBusy);
	printf("6e: The probability that exactly one of the nPSusers users is busy is pPSusers times the probability that a given specific user is transmitting and the remaining users are not transmitting = %le\n", pExactlyOne);
	printf("6f: The probability that 10 specific users of nPSusers are transmitting and the others are idle = %lg\n", pTenBusy);
	printf("6g: The probability that any 10 users of nPSusers are transmitting and the other are idle = %lg\n", pAnyTenBusy);
	printf("6h: The probability that more than 10 users of nPSusers are transmitting and the others are idle = %lg\n", pTenMoreBusy);

	return 0;
} 
