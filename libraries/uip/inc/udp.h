#ifndef UDP_H
#define UDP_H

/* ========================================================================== */

#include "../inc/ip.h"

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

int8_t udp_process_frame(
    struct udp*             self,
    const uint8_t*          rx_frame,
    uint16_t                rx_frame_size,
    struct udp_rx_metadata* mdata);

int8_t udp_buid_frame(
    struct udp*             self,
    struct udp_rx_metadata* mdata,
    uint8_t*                tx_frame,
    uint16_t                tx_frame_size);

/* ========================================================================== */

#endif /* UDP_H */
