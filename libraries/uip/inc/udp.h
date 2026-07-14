#ifndef UDP_H
#define UDP_H

/* ========================================================================== */

#include "ip.h"

#include <stdbool.h>
#include <stdint.h>

/* ========================================================================== */

struct udp
{
    uint16_t lost_frames;
};

struct udp_rx_metadata
{
    struct ip_rx_metadata* ip_mdata;
    uint16_t               src_port_num;
    uint16_t               dest_port_num;
    const uint8_t*         payload;
    uint16_t               payload_size;
};

struct udp_tx_metadata
{
    struct ip_tx_metadata* ip_mdata;
    uint16_t               src_port_num;
    uint16_t               dest_port_num;
    uint8_t*               payload;
    uint16_t               payload_size;
};

/* ========================================================================== */

/**
 * @brief Process a UDP frame. Verifies the checksum over the pseudo-header
 * and the UDP frame, and extracts header fields and payload pointer.
 * @param self Pointer to the udp object instance.
 * @param rx_frame Pointer to the UDP frame (without Ethernet and IP headers).
 * @param rx_frame_size Size of the UDP frame in bytes. Must be >=
 * UDP_HEADER_SIZE.
 * @param mdata Pointer to the rx metadata struct, where payload info will be
 * stored. Its ip_mdata member must already point to the IP rx metadata for
 * this frame.
 * @return int8_t Returns 0 in case of success, -ERRNO otherwise.
 */
int8_t udp_process_frame(
    struct udp*             self,
    const uint8_t*          rx_frame,
    uint16_t                rx_frame_size,
    struct udp_rx_metadata* mdata);

/**
 * @brief Build a UDP frame from the provided metadata. Copies the payload,
 * computes and writes the checksum over the pseudo-header and the UDP frame.
 * @param self Pointer to the udp object instance.
 * @param mdata Pointer to the tx metadata struct containing source/destination
 * ports, payload pointer and payload size. Its ip_mdata member must already
 * point to the IP tx metadata for this frame.
 * @param tx_frame Pointer to the output buffer where the UDP frame will be
 * written.
 * @param tx_frame_size Output parameter. Set to UDP_HEADER_SIZE + payload_size
 * on success.
 * @return int8_t Returns 0 in case of success, -ERRNO otherwise.
 */
int8_t udp_build_frame(
    struct udp*             self,
    struct udp_tx_metadata* mdata,
    uint8_t*                tx_frame,
    uint16_t*               tx_frame_size);

/* ========================================================================== */

#endif /* UDP_H */
