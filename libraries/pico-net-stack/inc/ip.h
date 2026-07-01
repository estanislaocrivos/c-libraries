#ifndef IP_H
#define IP_H

/* ========================================================================== */

#include <stdbool.h>
#include <stdint.h>

/* ========================================================================== */

enum IP_PLD_PROT_TYPE
{
    IP_PLD_UDP,
    IP_PLD_TCP,
    IP_PLD_ICMP,
    IP_PLD_UNKNOWN,
};

enum IP_VERSION
{
    IP_VER_4,
    IP_VER_6,
    IP_VER_UNKNOWN,
};

struct ip
{
    uint8_t ip_addr[4];
};

struct ip_rx_metadata
{
    enum IP_VERSION       version;
    enum IP_PLD_PROT_TYPE pld_prot_type;
    uint8_t               dest_ip[4];
    uint8_t               src_ip[4];
    const uint8_t*        payload;
    uint16_t              payload_size;
};

int8_t ip_process_frame(
    const struct ip*       self,
    const uint8_t*         rx_frame,
    uint16_t               rx_frame_size,
    struct ip_rx_metadata* mdata);

bool ip_is_pkt_for_me(const struct ip* self, const struct ip_rx_metadata* mdata);

/* ========================================================================== */

#endif
