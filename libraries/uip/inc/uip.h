#ifndef UIP_H
#define UIP_H

#include "eth.h"
#include "icmp.h"
#include "ip.h"
#include "udp.h"

#define IP_FRAME_OFST     ETH_HEADER_SIZE
#define L4_FRAME_OFST     (ETH_HEADER_SIZE + IP_HEADER_SIZE)
#define UDP_PAYLOAD_OFST  (L4_FRAME_OFST + UDP_HEADER_SIZE)
#define ICMP_PAYLOAD_OFST (L4_FRAME_OFST + ICMP_HEADER_SIZE)

#endif /* UIP_H */
