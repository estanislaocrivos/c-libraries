#ifndef ARP_H
#define ARP_H

/* ========================================================================== */

#include <stdbool.h>
#include <stdint.h>

/* ========================================================================== */

#define ARP_PACKET_SIZE 28

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
    uint8_t          dest_ip_addr[4];
};

struct arp_tx_metadata
{
    enum arp_op_type op_type;
    uint8_t          dest_ip_addr[4];
    uint8_t          dest_mac_addr[6];
};

/**
 * @brief Process an ARP frame.
 * @param self Pointer to the arp object instance.
 * @param rx_frame Pointer to the ARP frame.
 * @param rx_frame_size Size of the ARP frame to be processed.
 * @param mdata Pointer to the rx metadata struct, where payload info will be
 * stored.
 * @return int8_t Returns 0 in case of success, -ERRNO otherwise.
 */
int8_t arp_process_frame(
    const struct arp*       self,
    const uint8_t*          rx_frame,
    uint8_t                 rx_frame_size,
    struct arp_rx_metadata* mdata);

/**
 * @brief Build an ARP frame from the provided metadata.
 * @param self Pointer to the arp object instance.
 * @param mdata Pointer to the tx metadata struct containing op_type,
 * destination IP and destination MAC address.
 * @param tx_frame Pointer to the output buffer where the ARP frame will be
 * written. Must be at least ARP_PAYLOAD_SIZE bytes.
 * @param tx_frame_size Output parameter. Set to ARP_PAYLOAD_SIZE on success.
 * @return int8_t Returns 0 in case of success, -ERRNO otherwise.
 */
int8_t arp_build_frame(
    const struct arp*       self,
    struct arp_tx_metadata* mdata,
    uint8_t*                tx_frame,
    uint8_t*                tx_frame_size);

/**
 * @brief Check if an ARP request is addressed to this node. Must be called
 * after arp_process_frame in order to check the content of the metadata.
 * @param self Pointer to the arp object instance.
 * @param mdata Pointer to the rx metadata struct, where payload info is
 * stored.
 * @return bool Returns true if the request targets this node, false otherwise
 * or in case of error.
 */
bool arp_is_request_for_me(
    const struct arp* self, const struct arp_rx_metadata* mdata);

/* ========================================================================== */

#endif /* ARP_H */
