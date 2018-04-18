/*
 * libGedek.h
 *
 *  Created on: Jun 14, 2017
 *      Author: vvoisin
 */

#ifndef LIBGEDEK_H_
#define LIBGEDEK_H_

#include <stdint.h>
#include <netinet/in.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>

#define GEDEK_DEFAULT_PORT 1234

__BEGIN_DECLS
int printGedekHelp();

int parseArgvGedek(int argc, char *argv[], in_addr_t *destGedekIpAddress, struct ether_addr *destGedekMacAddress,  struct sockaddr_in *gedekDestinationIpPort1, struct ether_addr *destGedekDestinationMacAddress1, struct sockaddr_in *gedekDestinationIpPort2, struct ether_addr *destGedekDestinationMacAddress2);

int configureGedek(in_port_t gedekDefaultPort, struct sockaddr_in *gedekIpPort, struct ether_addr *srcDefaultGedekMacAddress, struct sockaddr_in *destinationIpPort1, struct ether_addr *destinationMacAdress1, struct sockaddr_in *destinationIpPort2, struct ether_addr *destinationMacAdress2);

int checkGedekConfig(struct sockaddr_in *gedekIpPort, struct ether_addr *srcDefaultGedekMacAddress, struct sockaddr_in *destinationIpPort1, struct ether_addr *destinationMacAdress1, struct sockaddr_in *destinationIpPort2, struct ether_addr *destinationMacAdress2);

int printGedekConfig(struct sockaddr_in *gedekIpPort, struct ether_addr *srcDefaultGedekMacAddress, struct sockaddr_in *destinationIpPort1, struct ether_addr *destinationMacAdress1, struct sockaddr_in *destinationIpPort2, struct ether_addr *destinationMacAdress2);

__END_DECLS

#endif /* LIBGEDEK_H_ */
