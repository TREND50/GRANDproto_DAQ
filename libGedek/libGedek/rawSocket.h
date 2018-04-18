/*
 * rawSocket.h
 *
 *  Created on: Jun 15, 2017
 *      Author: vvoisin
 */

#ifndef RAWSOCKET_H_
#define RAWSOCKET_H_

#include <netinet/in.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>

/* Define some constants. */
#define IP4_HDRLEN 20         // IPv4 header length
#define UDP_HDRLEN  8         // UDP header length, excludes data

#define LENMAC 6	//Number of bytes in a mac address

int get_interface_to_bind(in_addr_t *destHostInterfaceIp, in_addr_t *srcGedekDefaultIpAddress, struct sockaddr_ll *destllBindAddress);
uint64_t macAddrToUint64(struct ether_addr *srcMacAddr);

int open_rawsocket(struct sockaddr_ll *bindAddress);
int close_rawsocket(int sock);

int send_udp_to_rawsocket(int sock, const struct sockaddr_in *srcAddress, const struct sockaddr_in *destAddress, struct ether_addr *destMacAddress, const void *data, int dataLen, int srcTimeout);

#endif /* RAWSOCKET_H_ */
