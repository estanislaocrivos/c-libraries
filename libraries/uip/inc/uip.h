#ifndef UIP_H
#define UIP_H

#include "eth.h"
#include "icmp.h"
#include "ip.h"
#include "udp.h"

static const uint8_t IP_FRAME_OFST     = ETH_HEADER_SIZE;
static const uint8_t UDP_FRAME_OFST    = ETH_HEADER_SIZE + IP_HEADER_SIZE;
static const uint8_t ICMP_FRAME_OFST   = ETH_HEADER_SIZE + IP_HEADER_SIZE;
static const uint8_t UDP_PAYLOAD_OFST  = UDP_FRAME_OFST + UDP_HEADER_SIZE;
static const uint8_t ICMP_PAYLOAD_OFST = ICMP_FRAME_OFST + ICMP_HEADER_SIZE;

#endif /* UIP_H */
