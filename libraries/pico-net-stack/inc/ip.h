#ifndef IP_H
#define IP_H

/* ========================================================================== */

#include <stdbool.h>
#include <stdint.h>

/* ========================================================================== */

enum ip_pld_prot_type
{
    IP_PLD_UDP,
    IP_PLD_TCP,
    IP_PLD_ICMP,
    IP_PLD_UNKNOWN,
};

enum ip_version
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
    enum ip_version       version;
    enum ip_pld_prot_type pld_prot_type;
    uint8_t               dest_ip[4];
    uint8_t               src_ip[4];
    const uint8_t*        payload;
    uint16_t              payload_size;
};

/**
 * @brief Process an IPv4 frame.
 * @param self Pointer to the ip object instance.
 * @param rx_frame Pointer to the IPv4 frame (without Ethernet header).
 * @param rx_frame_size Size of the IPv4 frame. Must be >= 20 bytes.
 * @param mdata Pointer to the rx metadata struct, where payload info will be
 * stored.
 * @return int8_t Returns 0 in case of success, -ERRNO otherwise.
 */
int8_t ip_process_frame(
    const struct ip*       self,
    const uint8_t*         rx_frame,
    uint16_t               rx_frame_size,
    struct ip_rx_metadata* mdata);

/**
 * @brief Check if an IPv4 packet is addressed to this node. Must be called
 * after ip_process_frame in order to check the content of the metadata.
 * @param self Pointer to the ip object instance.
 * @param mdata Pointer to the rx metadata struct, where payload info is
 * stored.
 * @return bool Returns true if the packet is for this node, false otherwise.
 */
bool ip_is_pkt_for_me(const struct ip* self, const struct ip_rx_metadata* mdata);

/* ========================================================================== */

#endif /* IP_H */
