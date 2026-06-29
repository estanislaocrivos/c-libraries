#ifndef ARP_H
#define ARP_H

/* ========================================================================== */

#include <stdbool.h>
#include <stdint.h>

/* ========================================================================== */

enum arp_op_type
{
    ARP_REQUEST,
    ARP_REPLY,
    ARP_REV_REQUEST,
    ARP_REV_REPLY,
    ARP_UNKNOWN_TYPE,
};

struct arp
{
    uint8_t ip_addr[4];
    uint8_t mac_addr[6];
};

struct arp_rx_metadata
{
    enum arp_op_type op_type;
    uint8_t          src_ip_addr[4];
    uint8_t          src_mac_addr[6];
};

struct arp_tx_metadata
{
    enum arp_op_type op_type;
    uint8_t          dest_ip_addr[4];
    uint8_t          dest_mac_addr[6];
};

int8_t arp_process_frame(
    const struct arp*       self,
    const uint8_t*          rx_frame,
    uint8_t                 rx_frame_size,
    struct arp_rx_metadata* mdata);

int8_t arp_build_frame(
    const struct arp*       self,
    struct arp_tx_metadata* mdata,
    uint8_t*                tx_frame,
    uint8_t                 tx_frame_size);

/* ========================================================================== */

#endif /* ARP_H */
