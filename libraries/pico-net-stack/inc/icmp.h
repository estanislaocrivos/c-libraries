#ifndef ICMP_H
#define ICMP_H

#include <stdint.h>

#define ICMP_HDR_SIZE     8
#define ICMP_ECHO_REQUEST 8
#define ICMP_ECHO_REPLY   0

struct icmp
{
    int unused;
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

int8_t icmp_process_frame(
    const struct icmp*       self,
    const uint8_t*           rx_frame,
    uint16_t                 rx_frame_size,
    struct icmp_rx_metadata* mdata);

int8_t icmp_build_frame(
    const struct icmp*       self,
    struct icmp_tx_metadata* mdata,
    uint8_t*                 tx_frame,
    uint16_t*                tx_frame_size);

#endif
