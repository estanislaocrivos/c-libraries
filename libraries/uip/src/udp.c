#include "../inc/udp.h"

/* ========================================================================== */

#include "../../inc/errno.h"
#include "../inc/utils.h"

#include <stddef.h>

/* ========================================================================== */

#define UDP_HEADER_SIZE          8

#define UDP_SRC_PORT_FRAME_OFST  0 /* 2 bytes */
#define UDP_DEST_PORT_FRAME_OFST 2 /* 2 bytes */
#define UDP_LENGTH_FRAME_OFST    4 /* 2 bytes */
#define UDP_CHECKSUM_FRAME_OFST  6 /* 2 bytes */
#define UDP_PAYLOAD_FRAME_OFST \
    8 /* Payload size = UDP_LENGTH_FRAME_OFST - UDP_HEADER_SIZE  bytes */

/* ========================================================================== */

int8_t udp_process_frame(
    struct udp*             self,
    const uint8_t*          rx_frame,
    uint16_t                rx_frame_size,
    struct udp_rx_metadata* mdata)
{
    if (self == NULL || rx_frame == NULL || mdata == NULL)
    {
        return -EFAULT;
    }

    struct slice frame_slice = {.base = rx_frame, .len = rx_frame_size};

    /* Add IP addrs. before checksum (pseudo header) */
    if (compute_inet_checksum(&frame_slice, 1) != 0)
    {
        self->lost_frames += 1;
        return -EINVAL;
    }

    mdata->src_port_num = ((uint16_t)(rx_frame[UDP_SRC_PORT_FRAME_OFST]) << 8)
                          | (uint16_t)(rx_frame[UDP_SRC_PORT_FRAME_OFST + 1]);
    mdata->dest_port_num = ((uint16_t)(rx_frame[UDP_DEST_PORT_FRAME_OFST]) << 8)
                           | (uint16_t)(rx_frame[UDP_DEST_PORT_FRAME_OFST + 1]);
    mdata->payload      = rx_frame + UDP_PAYLOAD_FRAME_OFST;
    mdata->payload_size = rx_frame[UDP_LENGTH_FRAME_OFST] - UDP_HEADER_SIZE;
    return 0;
}

/* ========================================================================== */
