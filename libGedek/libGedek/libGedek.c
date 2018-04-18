/*
 * libGedek.c
 *
 *  Created on: Jun 14, 2017
 *      Author: vvoisin
 */


#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/ether.h>
#include "../libGedek/gedek.h"
#include "../libGedek/rawSocket.h"

int printGedekHelp() {
	fprintf(stderr, "Usage: configureGedek GEDEK_MAC_ADRESS NEW_GEDEK_IP DESTINATION_IP1:DESTINATION_PORT1 DESTINATION_MAC_ADDRESS1 DESTINATION_IP2:DESTINATION_PORT2 DESTINATION_MAC_ADDRESS2\n\n");
	fprintf(stderr, "GEDEK_MAC_ADRESS\t\t"			"MAC Address of the GEDEK we want to change Ip and other parameters\n"
					"NEW_GEDEK_IP\t\t\t"			"New GEDEK Ip to be assigned\n"
					"DESTINATION_IP1\t\t\t"			"Ip Address of the destination1\n"
					"DESTINATION_PORT1\t\t"			"Port Number of the destination1\n"
					"DESTINATION_MAC_ADDRESS1\t"	"MAC Address of the destination1 Network Card\n"
					"DESTINATION_IP2\t\t\t"			"Ip Address of the destination2\n"
					"DESTINATION_PORT2\t\t"			"Port Number of the destination2\n"
					"DESTINATION_MAC_ADDRESS2\t"	"MAC Address of the destination2 Network Card\n"
					"\n"
	);

	fprintf(stderr, "Example: configureGedek 00:07:11:a6:9e:c7 192.168.1.20 192.168.1.207:2000 80:3f:5d:70:6e:7c 192.168.1.207:2001 80:3f:5d:70:6e:7c\n");

	return (0);
}

int parseArgvGedek(int argc, char *argv[], in_addr_t *destGedekIpAddress, struct ether_addr *destGedekMacAddress,  struct sockaddr_in *gedekDestinationIpPort1, struct ether_addr *destGedekDestinationMacAddress1, struct sockaddr_in *gedekDestinationIpPort2, struct ether_addr *destGedekDestinationMacAddress2) {
	int c;
	int ret;
//	struct in_addr tmpIn;
	struct ether_addr *ret_ether_addr = NULL;
	char *port_number = NULL;
//	char strGedekMacAddress[] = "00:07:11:a6:9e:c7";
//	char strGedekIpAddress[] = "192.168.1.20";
//	char strGedekDestinationIpPort1[] = "192.168.1.207:2000";
//	char strGedekDestinationMac1[] = "80:3f:5d:70:6e:7c";
//	char strGedekDestinationIpPort2[] = "192.168.1.207:2001";
//	char strGedekDestinationIpAddress2[] = "80:3f:5d:70:6e:7c";

	char *strGedekMacAddress = NULL;
	char *strGedekIpAddress = NULL;
	char *strGedekDestinationIpPort1 = NULL;
	char *strGedekDestinationMac1 = NULL;
	char *strGedekDestinationIpPort2 = NULL;
	char *strGedekDestinationMac2 = NULL;

	while ((c = getopt(argc, argv, "h")) != -1) {
		switch (c) {
		case 'h':
			return (-1);
			break;
		case '?':
			break;
		default:
			break;
		}
	}

	if(argc != 7) {
		return (-1);
	}

	strGedekMacAddress = argv[1];
	strGedekIpAddress = argv[2];
	strGedekDestinationIpPort1 = argv[3];
	strGedekDestinationMac1 = argv[4];
	strGedekDestinationIpPort2 = argv[5];
	strGedekDestinationMac2 = argv[6];

/* destGedekIpAddress */
	ret = inet_pton(AF_INET, strGedekIpAddress, destGedekIpAddress);
	if (ret <= 0) {
		if (ret == 0) {
			fprintf(stderr, "New GEDEK Ip Address is Not in presentation format: %s\n", strGedekIpAddress);
		} else {
			perror("inet_pton");
			return (ret);
		}
	}
//	tmpIn.s_addr = *destGedekIpAddress;
//	printf("New GEDEK Ip Address: %s\n", inet_ntoa(tmpIn));

/* destGedekMacAddress */
	ret_ether_addr = ether_aton_r(strGedekMacAddress, destGedekMacAddress);
	if (ret_ether_addr == NULL) {
		fprintf(stderr, "GEDEK Mac Address is Not in presentation format: %s\n", strGedekMacAddress);
		return (-1);
	}
//	printf("GEDEK Mac Address is: %s\n", ether_ntoa(destGedekMacAddress));

/* gedekDestinationIpPort1 */
	/* port number */
	port_number = strchr(strGedekDestinationIpPort1, ':');
	if(port_number == NULL) {
		fprintf(stderr, "New GEDEK Destination1 IP Address and Port is Not in presentation format: %s\n", strGedekDestinationIpPort1);
		return (-2);
	}
	ret = sscanf(port_number+1, "%ud", (unsigned int*)&gedekDestinationIpPort1->sin_port);
	if(ret != 1) {
		fprintf(stderr, "New GEDEK Destination1 IP Address and Port is Not in presentation format: %s\n", port_number+1);
		return (-2);
	}
	gedekDestinationIpPort1->sin_port = htons(gedekDestinationIpPort1->sin_port);

	/* ip address */
	*(port_number) = '\0';
	ret = inet_pton(AF_INET, strGedekDestinationIpPort1, &gedekDestinationIpPort1->sin_addr.s_addr);
	if (ret <= 0) {
		if (ret == 0) {
			fprintf(stderr, "New GEDEK Destination1 IP Address and Port is Not in presentation format: %s\n", strGedekDestinationIpPort1);
		} else {
			perror("inet_pton");
			return (ret);
		}
	}
//	printf("New GEDEK Destination1 IP Address and Port is: %s:%d\n", inet_ntoa(gedekDestinationIpPort1->sin_addr), htons(gedekDestinationIpPort1->sin_port));


/* destGedekDestinationMacAddress1 */
	ret_ether_addr = ether_aton_r(strGedekDestinationMac1, destGedekDestinationMacAddress1);
	if (ret_ether_addr == NULL) {
		fprintf(stderr, "New GEDEK Destination2 Mac Address is Not in presentation format: %s\n", strGedekDestinationMac1);
		return (-1);
	}
//	printf("New GEDEK Destination2 Mac Address is: %s\n", ether_ntoa(destGedekDestinationMacAddress1));

/* gedekDestinationIpPort2 */
	/* port number */
	port_number = strchr(strGedekDestinationIpPort2, ':');
	if(port_number == NULL) {
		fprintf(stderr, "New GEDEK Destination2 IP Address and Port is Not in presentation format: %s\n", strGedekDestinationIpPort2);
		return (-2);
	}
	ret = sscanf(port_number+1, "%ud", (unsigned int*)&gedekDestinationIpPort2->sin_port);
	if(ret != 1) {
		fprintf(stderr, "New GEDEK Destination2 IP Address and Port is Not in presentation format: %s\n", port_number+1);
		return (-2);
	}
	gedekDestinationIpPort2->sin_port = htons(gedekDestinationIpPort2->sin_port);

	/* ip address */
	*(port_number) = '\0';
	ret = inet_pton(AF_INET, strGedekDestinationIpPort2, &gedekDestinationIpPort2->sin_addr.s_addr);
	if (ret <= 0) {
		if (ret == 0) {
			fprintf(stderr, "New GEDEK Destination1 IP Address and Port is Not in presentation format: %s\n", strGedekDestinationIpPort2);
		} else {
			perror("inet_pton");
			return (ret);
		}
	}
//	printf("New GEDEK Destination1 IP Address and Port is %s:%d\n", inet_ntoa(gedekDestinationIpPort2->sin_addr), htons(gedekDestinationIpPort2->sin_port));

/* destGedekDestinationMacAddress2 */
	ret_ether_addr = ether_aton_r(strGedekDestinationMac2, destGedekDestinationMacAddress2);
	if (ret_ether_addr == NULL) {
		fprintf(stderr, "New GEDEK Destination2 MAC Address is Not in presentation format: %s\n", strGedekDestinationMac2);
		return (-1);
	}
//	printf("New GEDEK Mac Address1 is: %s\n", ether_ntoa(destGedekDestinationMacAddress2));

	return (0);
}

int initGedekUdpPayLoad(RW readOrWrite, in_addr_t fpgaIp, struct ether_addr *fpgaMacAdress, struct sockaddr_in *destinationIp1, struct ether_addr *destinationMacAdress1, struct sockaddr_in *destinationIp2, struct ether_addr *destinationMacAdress2, TrendIntReg_t *destTrendIntReg) {
	uint64_t tmpMacAddr;
	int retToWrite;

	if(destTrendIntReg == NULL){
		return (-EFAULT);
	}

	memset(destTrendIntReg, 0, sizeof(TrendIntReg_t));

	/* HEADER / TRAILER */

	destTrendIntReg->Header					= TRENDINTREG_HEADERA;								/* data[0] */
	destTrendIntReg->Trailer				= TRENDINTREG_TRAILERA;								/* data[13] */
	destTrendIntReg->msgType				= TRENDINTREG_MSGTYPE;								/* data[1] */
	destTrendIntReg->Y						= readOrWrite;										/* data[2] Y=1 write the following registers (Y=0 read back) */

	if(readOrWrite == RW_WRITE) {
		tmpMacAddr								= macAddrToUint64(fpgaMacAdress);
		destTrendIntReg->gedekMac				= (uint32_t)(tmpMacAddr & 0xFFFFFFFF);			/* data[3] */

		destTrendIntReg->gedekIp				= htonl(fpgaIp);										/* data[4] */

		tmpMacAddr								= macAddrToUint64(destinationMacAdress1);
		destTrendIntReg->destinationMac1_low	= (uint32_t)(tmpMacAddr & 0xFFFFFFFF);			/* data[5] */
		destTrendIntReg->destinationMac1_high	= (uint32_t)((tmpMacAddr >> 32) & 0xFFFFFFFF);	/* data[6] */

		destTrendIntReg->destinationIp1			= htonl(destinationIp1->sin_addr.s_addr);				/* data[7] */
		destTrendIntReg->port1					= htons(destinationIp1->sin_port);						/* data[11] */

		tmpMacAddr								= macAddrToUint64(destinationMacAdress2);
		destTrendIntReg->destinationMac2_low	= (uint32_t)(tmpMacAddr & 0xFFFFFFFF);			/* data[8] */
		destTrendIntReg->destinationMac2_high	= (uint32_t)((tmpMacAddr >> 32) & 0xFFFFFFFF);	/* data[9] */

		destTrendIntReg->destinationIp2			= htonl(destinationIp2->sin_addr.s_addr);				/* data[10] */
		destTrendIntReg->port2					= htons(destinationIp2->sin_port);						/* data[12] */

		destTrendIntReg->Trailer				= TRENDINTREG_TRAILERA;
		retToWrite								= sizeof(destTrendIntReg->data);
	}

	if(readOrWrite == RW_READ) {
		destTrendIntReg->Y						= TRENDINTREG_Y_READBACK;
		destTrendIntReg->askTrailer				= TRENDINTREG_TRAILERA;
		retToWrite								= TRENDINTREG_Y0_SIZE * sizeof(uint32_t);
	}

	return (retToWrite);

}


int configureGedek(in_port_t gedekDefaultPort, struct sockaddr_in *gedekIpPort, struct ether_addr *srcDefaultGedekMacAddress, struct sockaddr_in *destinationIpPort1, struct ether_addr *destinationMacAdress1, struct sockaddr_in *destinationIpPort2, struct ether_addr *destinationMacAdress2) {
	int ret;
	struct sockaddr_in hostInterfaceIp;
	struct sockaddr_in broadcastIpPort;
	int raw_socket;
	int payloadLen;
	TrendIntReg_t destTrendIntReg;

	gedekIpPort->sin_port = htons(gedekDefaultPort);

	broadcastIpPort.sin_addr.s_addr = gedekIpPort->sin_addr.s_addr;
	broadcastIpPort.sin_port = gedekIpPort->sin_port;

	ret = get_interface_to_bind(&hostInterfaceIp.sin_addr.s_addr, &broadcastIpPort.sin_addr.s_addr, NULL);
	if(ret < 0) {
		perror("get_interface_to_bind Failed");
		return (ret);
	}

//	inet_pton(AF_INET, "192.168.1.18", &broadcastIpPort.sin_addr.s_addr);

	raw_socket = open_rawsocket(NULL);
	if(raw_socket < 0) {
		perror("open_rawsocket Failed");
		raw_socket = 0;
//		return (raw_socket);
	}

	payloadLen = initGedekUdpPayLoad(RW_WRITE, gedekIpPort->sin_addr.s_addr, srcDefaultGedekMacAddress, destinationIpPort1, destinationMacAdress1, destinationIpPort2, destinationMacAdress2, &destTrendIntReg);

	ret = send_udp_to_rawsocket(raw_socket, &hostInterfaceIp, &broadcastIpPort, srcDefaultGedekMacAddress, (void*)&destTrendIntReg , payloadLen, 0);
	if(ret < 0) {
		close_rawsocket(raw_socket);
		perror("send_udp_to_rawsocket Failed");
		return (ret);
	}

	ret = close_rawsocket(raw_socket);
	if(ret < 0) {
		perror("close_rawsocket Failed");
		return (ret);
	}

	return (ret);
}


ssize_t snprintfMacAdress(char *destBuf, size_t destBufSize, const struct ether_addr *macAddress) {
	char *ptrDestBuf = NULL;
	int mac;
	ssize_t nbSprintf;

	ptrDestBuf = destBuf;
	for (mac = 0; mac < ETH_ALEN; mac++) {
		ptrDestBuf += snprintf(ptrDestBuf, destBufSize - (ptrDestBuf - destBuf),
				"%02X:", macAddress->ether_addr_octet[mac]);
	}

	if ((ptrDestBuf - destBuf) > 0) {
		ptrDestBuf--;		//Remove last ':' (%02X:)
		*ptrDestBuf = '\0';	//Remove last ':' (%02X:)
	}

	nbSprintf = strlen(destBuf);

	return (nbSprintf);
}


int checkGedekConfig(struct sockaddr_in *gedekIpPort, struct ether_addr *srcDefaultGedekMacAddress, struct sockaddr_in *destinationIpPort1, struct ether_addr *destinationMacAdress1, struct sockaddr_in *destinationIpPort2, struct ether_addr *destinationMacAdress2) {
	int sock;
	int ret;
	struct sockaddr_in bindAddress;
	socklen_t addr_in_size;
	int payloadLen;
	TrendIntReg_t destTrendIntReg;
	fd_set readfds;
	struct timeval tv_timeout;
	char destBuf[BUFSIZ];
	struct sockaddr_in recvFromAddress;
	TrendRdIntReg_t *trendRdIntReg = NULL;
	TrendAck_t *trendAck = NULL;
	uint64_t gedekMacAddress;
	uint64_t readMacAdddress;
	uint64_t destinationMac;

	addr_in_size = sizeof(struct sockaddr_in);

/* Open socket */
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock < 0) {
		perror("socket Failed");
		return (sock);
	}

	memset(&bindAddress, 0, addr_in_size);
	bindAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	bindAddress.sin_port = destinationIpPort2->sin_port;

/* Bind Slow Control Port */
	ret = bind(sock, (struct sockaddr*)&bindAddress, addr_in_size);
	if (ret < 0) {
		perror("bind Failed");
		close(sock);
		return (ret);
	}

/* send IntReg Request */
	payloadLen = initGedekUdpPayLoad(RW_READ, gedekIpPort->sin_addr.s_addr, srcDefaultGedekMacAddress, destinationIpPort1, destinationMacAdress1, destinationIpPort2, destinationMacAdress2, &destTrendIntReg);

	gedekIpPort->sin_family = AF_INET;

	ret = sendto(sock, (void*)&destTrendIntReg, payloadLen, 0, (struct sockaddr*)gedekIpPort, addr_in_size);
	if (ret < 0) {
		perror("sendto Failed");
		close(sock);
		return (ret);
	}

/* wait 1 seconde for TrendACK Message */
	FD_ZERO(&readfds);
	FD_SET(sock, &readfds);
	tv_timeout.tv_sec = 1;
	tv_timeout.tv_usec = 0;

	ret = select(sock+1, &readfds, NULL, NULL, &tv_timeout);
    if (ret < 0) {
    	perror("select Failed");
    	close(sock);
    	return (ret);
    }

    if(ret == 0) {
    	fprintf(stderr, "No Ack TRENDACK to TRENDRDINTREG request\n");
    	close(sock);
    	return (-1);
    }

    ret = recvfrom(sock, destBuf, sizeof(TrendAck_t), 0, (struct sockaddr *) &recvFromAddress, &addr_in_size);

    if(ret < 0) {
    	perror("recvfrom Failed");
    	close(sock);
        return (ret);
    }

    if(ret != sizeof(TrendAck_t)) {
    	fprintf(stderr, "Wrong Ack TRENDACK to TRENDRDINTREG request\n");
    	close(sock);
    	return (-1);
    }

    if(recvFromAddress.sin_addr.s_addr != gedekIpPort->sin_addr.s_addr) {
       	fprintf(stderr, "Wrong device has Ack to TRENDINTREG request\n");
       	close(sock);
       	return (-1);
    }

    trendAck = (TrendAck_t*)destBuf;

    /* check trendAck->Header */
	if(trendAck->Header != TRENDACK_HEADERA) {
		fprintf(stderr, "Wrong TRENDACK_HEADERA in Ack to TRENDINTREG request\n");
		close(sock);
		return (-1);
	}

	/* check trendAck->Trailer */
	if(trendAck->Trailer != TRENDACK_TRAILERA) {
		fprintf(stderr, "Wrong TRENDACK_TRAILERA in Ack to TRENDINTREG request\n");
		close(sock);
		return (-1);
	}

	/* check trendAck->msgType */
	if(trendAck->msgType != TRENDACK_MSGTYPE) {
		fprintf(stderr, "Wrong TRENDACK_MSGTYPE in Ack to TRENDINTREG request\n");
	    close(sock);
	    return (-1);
	}

    /* check trendAck->IpNum */
	if(gedekIpPort->sin_addr.s_addr != htonl(trendAck->IpNum)) {
		fprintf(stderr, "Wrong device has Acked to TRENDINTREG request\n");
		close(sock);
		return (-1);
	}

	/* check trendAck->MsgAck */
	if(trendAck->MsgAck != TRENDINTREG_MSGTYPE) {
		fprintf(stderr, "Wrong TRENDACK_MSGACK in Ack to TRENDINTREG request\n");
	    close(sock);
	    return (-1);
	}

/* wait 1 seconde for TrendRdIntReg Message */
	tv_timeout.tv_sec = 1;
	tv_timeout.tv_usec = 0;

	ret = select(sock+1, &readfds, NULL, NULL, &tv_timeout);
	if (ret < 0) {
		perror("select Failed");
		close(sock);
		return (ret);
	}

	if(ret == 0) {
		fprintf(stderr, "No TRENDRDINTREG answer to TRENDINTREG request\n");
		close(sock);
		return (-1);
	}

    ret = recvfrom(sock, destBuf, sizeof(TrendRdIntReg_t), 0, (struct sockaddr *) &recvFromAddress, &addr_in_size);
    if(recvfrom < 0) {
        perror("recvfrom Failed");
        close(sock);
        return (ret);
    }

    if(ret != sizeof(TrendRdIntReg_t)) {
    	fprintf(stderr, "Wrong Answer to TRENDINTREG request\n");
    	close(sock);
    	return (-1);
    }

    if(recvFromAddress.sin_addr.s_addr != gedekIpPort->sin_addr.s_addr) {
    	fprintf(stderr, "Wrong device has answered to TRENDINTREG request\n");
    	close(sock);
    	return (-1);
    }

    trendRdIntReg = (TrendRdIntReg_t*)destBuf;

    /* check trendRdIntReg->Header */
    if(trendRdIntReg->Header != TRENDRDINTREG_HEADERA) {
    	fprintf(stderr, "Wrong TRENDRDINTREG_HEADERA in Answer to TRENDINTREG request\n");
    	close(sock);
    	return (-1);
    }

    /* check trendRdIntReg->Trailer */
    if(trendRdIntReg->Trailer != TRENDRDINTREG_TRAILERA) {
    	fprintf(stderr, "Wrong TRENDRDINTREG_TRAILERA in Answer to TRENDINTREG request\n");
    	close(sock);
    	return (-1);
    }

    /* check trendRdIntReg->msgType */
    if(trendRdIntReg->msgType != TRENDRDINTREG_MSGTYPE) {
    	fprintf(stderr, "Wrong TRENDRDINTREG_MSGTYPE in Answer to TRENDINTREG request\n");
    	close(sock);
    	return (-1);
    }

    /* check trendRdIntReg->IpNum */
	if(gedekIpPort->sin_addr.s_addr != htonl(trendRdIntReg->IpNum)) {
		fprintf(stderr, "Wrong device has answered to TRENDINTREG request\n");
		close(sock);
		return (-1);
	}

	/* check trendRdIntReg->fpgaMac */
    gedekMacAddress = macAddrToUint64(srcDefaultGedekMacAddress);
    if((gedekMacAddress&0xFFFFFFFF) != trendRdIntReg->gedekMac) {
    	fprintf(stderr, "Wrong Gedek MAC address in TRENDINTREG request\n");
 //   	close(sock);
 //   	return (-1);
    }

    /* check trendRdIntReg->gedekIp */
    if(gedekIpPort->sin_addr.s_addr != htonl(trendRdIntReg->gedekIp)) {
    	fprintf(stderr, "Wrong Gedek IP address device in TRENDINTREG request\n");
    	close(sock);
    	return (-1);
    }

    /* check trendRdIntReg->trendRdIntReg->destinationMac1 */
    readMacAdddress = (uint64_t)trendRdIntReg->destinationMac1_low;
    readMacAdddress |= (uint64_t)trendRdIntReg->destinationMac1_high << 32;

    destinationMac = macAddrToUint64(destinationMacAdress1);
    if(destinationMac != readMacAdddress) {
    	fprintf(stderr, "Wrong MAC Destination1 address in TRENDINTREG request: %lX\n", readMacAdddress);
//    	close(sock);
//    	return (-1);
    }

    /* check trendRdIntReg->destinationIp1 */
    if(destinationIpPort1->sin_addr.s_addr != htonl(trendRdIntReg->destinationIp1)) {
    	fprintf(stderr, "Wrong Gedek Destination1 IP address device in TRENDINTREG request\n");
    	close(sock);
    	return (-1);
    }

    /* check trendRdIntReg->trendRdIntReg->destinationMac2 */
	readMacAdddress = (uint64_t)trendRdIntReg->destinationMac2_low;
	readMacAdddress |= (uint64_t)trendRdIntReg->destinationMac2_high << 32;

	destinationMac = macAddrToUint64(destinationMacAdress2);
	if(destinationMac != readMacAdddress) {
		fprintf(stderr, "Wrong MAC Destination2 address in TRENDINTREG request: %lX\n", readMacAdddress);
//		close(sock);
//		return (-1);
	}

	/* check trendRdIntReg->destinationIp1 */
	if(destinationIpPort2->sin_addr.s_addr != htonl(trendRdIntReg->destinationIp2)) {
		fprintf(stderr, "Wrong Gedek Destination2 IP address device in TRENDINTREG request\n");
		close(sock);
		return (-1);
	}

	/* check trendRdIntReg->destinationPort1 */
	if(destinationIpPort1->sin_port != htons(trendRdIntReg->destinationPort1)) {
		fprintf(stderr, "Wrong Gedek Destination1 Port Number in TRENDINTREG request: %d\n", trendRdIntReg->destinationPort1);
//		close(sock);
//		return (-1);
	}

	/* check trendRdIntReg->destinationPort2 */
	if(destinationIpPort2->sin_port != htons(trendRdIntReg->destinationPort2)) {
		fprintf(stderr, "Wrong Gedek Destination2 Port Number in TRENDINTREG request: %d\n", trendRdIntReg->destinationPort2);
//		close(sock);
//		return (-1);
	}

	return (ret);
}

int printGedekConfig(struct sockaddr_in *gedekIpPort, struct ether_addr *srcDefaultGedekMacAddress, struct sockaddr_in *destinationIpPort1, struct ether_addr *destinationMacAdress1, struct sockaddr_in *destinationIpPort2, struct ether_addr *destinationMacAdress2) {
	char printBuf[BUFSIZ];

	fprintf(stdout, "GEDEK MAC ADDRESS: ");
    snprintfMacAdress(printBuf, sizeof(printBuf), srcDefaultGedekMacAddress);
    fprintf(stdout, "%s\n", printBuf);

    fprintf(stdout, "GEDEK IP ADDRESS: %s\n", inet_ntoa(gedekIpPort->sin_addr));

    fprintf(stdout, "GEDEK DESTINATION1 IP ADDRESS: %s:%d\n", inet_ntoa(destinationIpPort1->sin_addr), htons(destinationIpPort1->sin_port));
	fprintf(stdout, "GEDEK DESTINATION1 MAC ADDRESS: ");
    snprintfMacAdress(printBuf, sizeof(printBuf), destinationMacAdress1);
    fprintf(stdout, "%s\n", printBuf);

    fprintf(stdout, "GEDEK DESTINATION2 IP ADDRESS: %s:%d\n", inet_ntoa(destinationIpPort2->sin_addr), htons(destinationIpPort2->sin_port));
	fprintf(stdout, "GEDEK DESTINATION2 MAC ADDRESS: ");
    snprintfMacAdress(printBuf, sizeof(printBuf), destinationMacAdress2);
    fprintf(stdout, "%s\n", printBuf);

    return (0);
}

