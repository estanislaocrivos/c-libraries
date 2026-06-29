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

struct eth_rx_metadata
{
    enum eth_payload_type payload_type;
    enum eth_mac_type     mac_type;
    uint8_t*              sender_mac_addr;
    uint8_t*              payload;
    uint16_t              payload_size;
};

struct eth_tx_metadata
{
    enum eth_payload_type payload_type;
    uint8_t*              dest_mac_addr;
    uint8_t*              payload;
    uint16_t              payload_size;
};

/**
 * @brief Process an ethernet frame
 * @param self Pointer to the eth object instance
 * @param rx_frame Pointer to the received ETH frame
 * @param rx_frame_size Size of the frame to be processed (>= 64 && <= 1518)
 * @param mdata Pointer to the returned rx_frame metadata
 * @return int8_t Returns 0 in case of success, -ERRNO otherwise
 */
int8_t eth_process_frame(
    const struct eth*       self,
    const uint8_t*          rx_frame,
    uint16_t                rx_frame_size,
    struct eth_rx_metadata* mdata);

int8_t eth_build_frame(
    const struct eth*       self,
    struct eth_tx_metadata* mdata,
    uint8_t*                tx_frame,
    uint16_t*               tx_frame_size);

/* ========================================================================== */

#endif
