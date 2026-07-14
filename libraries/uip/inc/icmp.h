#ifndef ICMP_H
#define ICMP_H

/* ========================================================================== */

#include <stdint.h>

/* ========================================================================== */

#define ICMP_HDR_SIZE     8
#define ICMP_ECHO_REQUEST 8
#define ICMP_ECHO_REPLY   0

/* ========================================================================== */

struct icmp
{
    uint16_t lost_frames;
};

struct icmp_rx_metadata
{
    uint8_t        type;
    uint8_t        code;
    uint16_t       id;
    uint16_t       seq_num;
    const uint8_t* payload;
    uint16_t       payload_size;
};

struct icmp_tx_metadata
{
    uint8_t        type;
    uint8_t        code;
    uint16_t       id;
    uint16_t       seq_num;
    const uint8_t* payload;
    uint16_t       payload_size;
};

/* ========================================================================== */

/**
 * @brief Process an ICMP frame. Verifies the Internet checksum over the entire
 * frame and extracts header fields and payload pointer.
 * @param self Pointer to the icmp object instance.
 * @param rx_frame Pointer to the ICMP frame (without Ethernet and IP headers).
 * @param rx_frame_size Size of the ICMP frame in bytes. Must be >=
 * ICMP_HDR_SIZE.
 * @param mdata Pointer to the rx metadata struct, where payload info will be
 * stored.
 * @return int8_t Returns 0 in case of success, -ERRNO otherwise.
 */
int8_t icmp_process_frame(
    struct icmp*             self,
    const uint8_t*           rx_frame,
    uint16_t                 rx_frame_size,
    struct icmp_rx_metadata* mdata);

/**
 * @brief Build an ICMP frame from the provided metadata. Copies the payload,
 * computes and writes the Internet checksum.
 * @param self Pointer to the icmp object instance.
 * @param mdata Pointer to the tx metadata struct containing type, code, id,
 * seq_num, payload pointer and payload size.
 * @param tx_frame Pointer to the output buffer where the ICMP frame will be
 * written.
 * @param tx_frame_size Output parameter. Set to ICMP_HDR_SIZE + payload_size
 * on success.
 * @return int8_t Returns 0 in case of success, -ERRNO otherwise.
 */
int8_t icmp_build_frame(
    const struct icmp*       self,
    struct icmp_tx_metadata* mdata,
    uint8_t*                 tx_frame,
    uint16_t*                tx_frame_size);

/* ========================================================================== */

#endif /* ICMP_H */
