#ifndef ETH_H
#define ETH_H

/* ========================================================================== */

#include <stdbool.h>
#include <stdint.h>

/* ========================================================================== */

enum eth_payload_type
{
    IPV4_PAYLOAD,
    ARP_PAYLOAD,
};

enum eth_mac_type
{
    BROADCAST_MAC,
    MULTICAST_MAC,
    MY_OWN_MAC,
};

struct eth
{
    uint8_t ip_addr[4];
    uint8_t mac_addr[6];
    bool    calc_crc;
};

struct eth_payload
{
    enum eth_payload_type pld_type;
    enum eth_mac_type     mac_type;
    uint8_t*              payload;
    uint16_t              size;
};

/**
 * @brief Process an ethernet frame
 *
 * @param self
 * @param frame
 * @param size Size of the frame to be processed (>= 64 && <= 1518)
 * @return int8_t
 */
int8_t eth_process_frame(
    const struct eth*   self,
    uint8_t*            frame,
    uint16_t            size,
    struct eth_payload* pld);

/* ========================================================================== */

#endif
