#ifndef UDP_H
#define UDP_H

/* ========================================================================== */

#include "../inc/ip.h"

#include <stdbool.h>
#include <stdint.h>

/* ========================================================================== */

struct udp
{
    uint16_t   lost_frames;
    struct ip* ip;
};

struct udp_rx_metadata
{
    uint16_t       src_port_num;
    uint16_t       dest_port_num;
    uint16_t       payload_size;
    const uint8_t* payload;
};

/* ========================================================================== */

int8_t udp_process_frame(
    struct udp*             self,
    const uint8_t*          rx_frame,
    uint16_t                rx_frame_size,
    struct udp_rx_metadata* mdata);

/* ========================================================================== */

#endif /* UDP_H */
