/*
 * rawSocket.c
 *
 *  Created on: Jun 15, 2017
 *      Author: vvoisin
 */


#include "../libGedek/rawSocket.h"

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <netinet/if_ether.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <net/if.h>

int get_interface_to_bind(in_addr_t *destHostInterfaceIp, in_addr_t *srcGedekDefaultIpAddress, struct sockaddr_ll *destllBindAddress) {
	struct ifaddrs *ifaddr, *ifa;
	int family, s, n;

	in_addr_t srcIpAddress;
	in_addr_t destIpAddress;
	in_addr_t interfaceIp;
	in_addr_t interfaceNetMask;
	int ifindex;
	int lastIfindex;

	int retCode;

	char host[NI_MAXHOST];

	if (getifaddrs(&ifaddr) == -1) {
		perror("getifaddrs");
		return (-1);
	}

	srcIpAddress = *srcGedekDefaultIpAddress;
	destIpAddress = srcIpAddress;

	lastIfindex = -1;
	retCode = 0;
	for (ifa = ifaddr, n = 0; ifa != NULL; ifa = ifa->ifa_next, n++) {
		if (ifa->ifa_addr == NULL)
			continue;

		family = ifa->ifa_addr->sa_family;

		/* Display interface name and family (including symbolic
		 form of the latter for the common families) */

//		printf("%-8s %s (%d)\n", ifa->ifa_name,
//				(family == AF_PACKET) ? "AF_PACKET" :
//				(family == AF_INET) ? "AF_INET" :
//				(family == AF_INET6) ? "AF_INET6" : "???", family);

		/* For an AF_INET* interface address, display the address */

		if (family == AF_INET) {
			interfaceIp = ((struct sockaddr_in*)(ifa->ifa_addr))->sin_addr.s_addr;
			interfaceNetMask = (((struct sockaddr_in*)(ifa->ifa_netmask))->sin_addr.s_addr);

			s = getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in), host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
			if (s != 0) {
				printf("getnameinfo() failed: %s\n", gai_strerror(s));
				return (s);
			}

//			printf("\t\taddress: <%s>\n", host);

			if((srcIpAddress & interfaceNetMask) == (interfaceIp & interfaceNetMask)) {
				ifindex = if_nametoindex(ifa->ifa_name);
				if(lastIfindex != -1) {
					retCode = -1;
				}
				lastIfindex = ifindex;
				destIpAddress = (srcIpAddress | ~interfaceNetMask);

				if(destHostInterfaceIp != NULL) {
					*destHostInterfaceIp = interfaceIp;
				}
			}
		}
	}

	freeifaddrs(ifaddr);
	ifaddr = NULL;

	*srcGedekDefaultIpAddress = destIpAddress;

	if(retCode > -1) {
		retCode = lastIfindex;
	}

	if ((destllBindAddress != NULL) && (lastIfindex != -1)) {
		memset(destllBindAddress, 0, sizeof(struct sockaddr_ll));
		destllBindAddress->sll_family = PF_PACKET;
		destllBindAddress->sll_protocol = htons(ETH_P_IP);
		destllBindAddress->sll_ifindex = lastIfindex;
		destllBindAddress->sll_halen = ETH_ALEN;
	}

	return (retCode);
}

int open_rawsocket(struct sockaddr_ll *bindAddress) {
	int sock;
	int retCode;
	socklen_t addr_in_size;

	// Open raw socket descriptor.
	if ((sock = socket(PF_PACKET, SOCK_DGRAM, htons(ETH_P_IP))) < 0) {
		perror("socket() failed ");
		return (sock);
	}

	addr_in_size = sizeof(struct sockaddr_ll);

	if (bindAddress != NULL) {
		/* bind socket to network interface (to read from udp)*/
		retCode = bind(sock, (struct sockaddr*) bindAddress, addr_in_size);
		if (retCode < 0) {
			perror("bind() failed ");
			close(sock);
			return (retCode);
		}
	}

	return (sock);
}

int close_rawsocket(int sock) {
	int ret;

	ret =  close(sock);
	if (ret < 0) {
		perror("close() failed ");
	}

	return (ret);
}

// Checksum function
uint16_t ipHeaderChecksum (uint16_t *addr, int len)
{
  int nleft = len;
  int sum = 0;
  uint16_t *w = addr;
  uint16_t answer = 0;

  while (nleft > 1) {
    sum += *w++;
    nleft -= sizeof (uint16_t);
  }

  if (nleft == 1) {
    *(uint8_t *) (&answer) = *(uint8_t *) w;
    sum += answer;
  }

  sum = (sum >> 16) + (sum & 0xFFFF);
  sum += (sum >> 16);
  answer = ~sum;
  return (answer);
}

// Build IPv4 UDP pseudo-header and call checksum function.
uint16_t initUdp4Header(struct ip ipHdr, struct udphdr udpHdr, const uint8_t *data, int dataLen) {
	char buf[IP_MAXPACKET];
	char *ptr;
	int chksumlen = 0;
	int i;

	ptr = &buf[0];  // ptr points to beginning of buffer buf

	// Copy source IP address into buf (32 bits)
	memcpy(ptr, &ipHdr.ip_src.s_addr, sizeof(ipHdr.ip_src.s_addr));
	ptr += sizeof(ipHdr.ip_src.s_addr);
	chksumlen += sizeof(ipHdr.ip_src.s_addr);

	// Copy destination IP address into buf (32 bits)
	memcpy(ptr, &ipHdr.ip_dst.s_addr, sizeof(ipHdr.ip_dst.s_addr));
	ptr += sizeof(ipHdr.ip_dst.s_addr);
	chksumlen += sizeof(ipHdr.ip_dst.s_addr);

	// Copy zero field to buf (8 bits)
	*ptr = 0;
	ptr++;
	chksumlen += 1;

	// Copy transport layer protocol to buf (8 bits)
	memcpy(ptr, &ipHdr.ip_p, sizeof(ipHdr.ip_p));
	ptr += sizeof(ipHdr.ip_p);
	chksumlen += sizeof(ipHdr.ip_p);

	// Copy UDP length to buf (16 bits)
	memcpy(ptr, &udpHdr.len, sizeof(udpHdr.len));
	ptr += sizeof(udpHdr.len);
	chksumlen += sizeof(udpHdr.len);

	// Copy UDP source port to buf (16 bits)
	memcpy(ptr, &udpHdr.source, sizeof(udpHdr.source));
	ptr += sizeof(udpHdr.source);
	chksumlen += sizeof(udpHdr.source);

	// Copy UDP destination port to buf (16 bits)
	memcpy(ptr, &udpHdr.dest, sizeof(udpHdr.dest));
	ptr += sizeof(udpHdr.dest);
	chksumlen += sizeof(udpHdr.dest);

	// Copy UDP length again to buf (16 bits)
	memcpy(ptr, &udpHdr.len, sizeof(udpHdr.len));
	ptr += sizeof(udpHdr.len);
	chksumlen += sizeof(udpHdr.len);

	// Copy UDP checksum to buf (16 bits)
	// Zero, since we don't know it yet
	*ptr = 0;
	ptr++;
	*ptr = 0;
	ptr++;
	chksumlen += 2;

	// Copy payload to buf
	memcpy(ptr, data, dataLen);
	ptr += dataLen;
	chksumlen += dataLen;

	// Pad to the next 16-bit boundary
	for (i = 0; i < dataLen % 2; i++, ptr++) {
		*ptr = 0;
		ptr++;
		chksumlen++;
	}

	return (ipHeaderChecksum((uint16_t *) buf, chksumlen));
}

int send_udp_to_rawsocket(int sock, const struct sockaddr_in *srcAddress, const struct sockaddr_in *destAddress, struct ether_addr *destMacAddress, const void *data, int dataLen, int srcTimeout) {
	int frame_length;
	struct sockaddr_in broadcastIpAddress;
	struct sockaddr_ll device;
	int retCode;
	int nbSended;
	struct ip ipHdr;
	int ip_flags[4];
	struct udphdr udpHdr;
	unsigned char ether_frame[IP_MAXPACKET];

	frame_length = IP4_HDRLEN + UDP_HDRLEN + dataLen;

	memset(&broadcastIpAddress, 0, sizeof(struct sockaddr_in));
	broadcastIpAddress.sin_addr.s_addr = srcAddress->sin_addr.s_addr;
	/* Fill out sockaddr_ll */
	retCode = get_interface_to_bind(NULL, &broadcastIpAddress.sin_addr.s_addr, &device);

	if(retCode < 0) {
		return (retCode);
	}

	memcpy(device.sll_addr, destMacAddress, sizeof(struct ether_addr));

	// IPv4 header
	// IPv4 header length (4 bits): Number of 32-bit words in header = 5
	ipHdr.ip_hl = IP4_HDRLEN / sizeof(uint32_t);

	// Internet Protocol version (4 bits): IPv4
	ipHdr.ip_v = 4;

	// Type of service (8 bits)
	ipHdr.ip_tos = 0;

	// Total length of datagram (16 bits): IP header + UDP header + datalen
	ipHdr.ip_len = htons(IP4_HDRLEN + UDP_HDRLEN + dataLen);

	// ID sequence number (16 bits): unused, since single datagram
	ipHdr.ip_id = htons(0);


	// Flags, and Fragmentation offset (3, 13 bits): 0 since single datagram

	// Zero (1 bit)
	ip_flags[0] = 0;

	// Do not fragment flag (1 bit)
	ip_flags[1] = 0;

	// More fragments following flag (1 bit)
	ip_flags[2] = 0;

	// Fragmentation offset (13 bits)
	ip_flags[3] = 0;

	ipHdr.ip_off = htons((ip_flags[0] << 15) + (ip_flags[1] << 14) + (ip_flags[2] << 13) + ip_flags[3]);

	// Time-to-Live (8 bits): default to maximum value
	ipHdr.ip_ttl = 255;

	// Transport layer protocol (8 bits): 17 for UDP
	ipHdr.ip_p = IPPROTO_UDP;

	// Source IPv4 address (32 bits)
	ipHdr.ip_src.s_addr = srcAddress->sin_addr.s_addr;
//	if ((status = inet_pton(AF_INET, src_ip, &(ipHdr.ip_src))) != 1) {
//		fprintf(stderr, "inet_pton() failed.\nError message: %s",
//				strerror(status));
//		exit(EXIT_FAILURE);
//	}

	// Destination IPv4 address (32 bits)
	ipHdr.ip_dst.s_addr = destAddress->sin_addr.s_addr;
//	if ((status = inet_pton(AF_INET, dst_ip, &(ipHdr.ip_dst))) != 1) {
//		fprintf(stderr, "inet_pton() failed.\nError message: %s",
//				strerror(status));
//		exit(EXIT_FAILURE);
//	}

	// IPv4 header checksum (16 bits): set to 0 when calculating checksum
	ipHdr.ip_sum = 0;
	ipHdr.ip_sum = ipHeaderChecksum((uint16_t *) &ipHdr, IP4_HDRLEN);

	// UDP header
	// Source port number (16 bits): pick a number
	udpHdr.source = srcAddress->sin_port;

	// Destination port number (16 bits): pick a number
	udpHdr.dest = destAddress->sin_port;

	// Length of UDP datagram (16 bits): UDP header + UDP data
	udpHdr.len = htons(UDP_HDRLEN + dataLen);

	// UDP checksum (16 bits)
	udpHdr.check = initUdp4Header(ipHdr, udpHdr, data, dataLen);

	/* Fill out ethernet frame header */

	/* Ethernet frame length = ethernet data (IP header + UDP header + UDP data) */
	frame_length = IP4_HDRLEN + UDP_HDRLEN + dataLen;

	/* IPv4 header */
	memcpy(ether_frame, &ipHdr, IP4_HDRLEN);

	/* UDP header */
	memcpy(ether_frame + IP4_HDRLEN, &udpHdr, UDP_HDRLEN);

	/* UDP data */
	memcpy(ether_frame + IP4_HDRLEN + UDP_HDRLEN, data, dataLen);

	/* Send ethernet frame to socket. */
	if ((nbSended = sendto(sock, ether_frame, frame_length, 0, (struct sockaddr *) &device, sizeof(device))) <= 0) {
		perror("sendto() failed");
		return (-errno);
	}

	return (nbSended);
}

uint64_t macAddrToUint64(struct ether_addr *srcMacAddr) {
	uint64_t macAddrQWORD;
	int mac;

	macAddrQWORD = 0;
	for(mac=0; mac<LENMAC; mac++) {
		macAddrQWORD |= ((uint64_t)(srcMacAddr->ether_addr_octet[LENMAC-1-mac])) << (8*mac);
	}

	return(macAddrQWORD);
}
