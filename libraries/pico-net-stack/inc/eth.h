#ifndef ETH_H
#define ETH_H

/* ========================================================================== */

#include <stdbool.h>
#include <stdint.h>

/* ========================================================================== */

enum eth_payload_type
{
    ETH_PLD_IPV4,
    ETH_PLD_IPV6,
    ETH_PLD_ARP,
    ETH_PLD_UNKNOWN,
};

enum eth_mac_type
{
    ETH_MAC_BROADCAST,
    ETH_MAC_MULTICAST,
    ETH_MAC_MY_OWN,
    ETH_MAC_UNKNOWN,
};

struct eth
{
    uint8_t ip_addr[4];
    uint8_t mac_addr[6];
    bool    calc_crc;
};

struct eth_metadata
{
    enum eth_payload_type payload_type;
    enum eth_mac_type     mac_type;
    uint8_t*              payload;
    uint16_t              payload_size;
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
    const struct eth*    self,
    uint8_t*             rx_frame,
    uint16_t             rx_frame_size,
    struct eth_metadata* meta);

/* ========================================================================== */

#endif
