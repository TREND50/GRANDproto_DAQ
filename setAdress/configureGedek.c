/*
 * main.c
 *
 *  Created on: Jun 14, 2017
 *      Author: vvoisin
 */


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <net/ethernet.h>
#include <netinet/in.h>
#include "libGedek/libGedek.h"


int main(int argc, char *argv[]) {
	int ret;
	struct sockaddr_in gedekIp;
	struct ether_addr defaultGedekMacAddress;
	struct sockaddr_in destinationIpPort1;
	struct ether_addr destinationMacAddress1;
	struct sockaddr_in destinationIpPort2;
	struct ether_addr destinationMacAddress2;

	ret = parseArgvGedek(argc, argv, &gedekIp.sin_addr.s_addr, &defaultGedekMacAddress, &destinationIpPort1, &destinationMacAddress1, &destinationIpPort2, &destinationMacAddress2);

	if(ret < 0) {
		printGedekHelp();
		exit(EXIT_FAILURE);
	}

	ret = configureGedek(GEDEK_DEFAULT_PORT, &gedekIp, &defaultGedekMacAddress, &destinationIpPort1, &destinationMacAddress1, &destinationIpPort2, &destinationMacAddress2);
	if(ret < 0) {
		fprintf(stderr, "FAILED TO CONFIGURE GEDEK\n");
//		exit(EXIT_FAILURE);
	}

	int try;
	printf("Waiting the GEDEK to be configured\n");
	for(try = 0; try<10; try++) {
		ret = checkGedekConfig(&gedekIp, &defaultGedekMacAddress, &destinationIpPort1, &destinationMacAddress1, &destinationIpPort2, &destinationMacAddress2);
		if(ret > -1) {
			break;
		}
		sleep(1);
	}

	if(ret > 0) {
		printGedekConfig(&gedekIp, &defaultGedekMacAddress, &destinationIpPort1, &destinationMacAddress1, &destinationIpPort2, &destinationMacAddress2);
	} else {
		fprintf(stderr, "FAILED TO READ GEDEK CONFIGURATION\n");
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}
