#include "../inc/udp.h"

/* ========================================================================== */

#include "../../inc/errno.h"
#include "../inc/utils.h"
#include "ip.h"

#include <stddef.h>

/* ========================================================================== */

#define UDP_HEADER_SIZE          8

#define IP_PLD_PROT_UDP_VAL      (uint8_t)17

#define UDP_SRC_PORT_FRAME_OFST  0 /* 2 bytes */
#define UDP_DEST_PORT_FRAME_OFST 2 /* 2 bytes */
#define UDP_LENGTH_FRAME_OFST    4 /* 2 bytes */
#define UDP_CHECKSUM_FRAME_OFST  6 /* 2 bytes */
#define UDP_PAYLOAD_FRAME_OFST \
    8 /* Payload size = UDP_LENGTH_FRAME_OFST - UDP_HEADER_SIZE  bytes */

/* ========================================================================== */

static uint16_t compute_udp_checksum(
    const uint8_t* src_ip,
    const uint8_t* dest_ip,
    const uint8_t* rx_frame,
    uint16_t       rx_frame_size)
{
    uint8_t udp_pseudo_header[4]
        = {0,
           IP_PLD_PROT_UDP_VAL,
           rx_frame[UDP_LENGTH_FRAME_OFST],
           rx_frame[UDP_LENGTH_FRAME_OFST + 1]};
    struct slice frame_slice[]
        = {{.base = src_ip, .len = 4},
           {.base = dest_ip, .len = 4},
           {.base = udp_pseudo_header, .len = sizeof(udp_pseudo_header)},
           {.base = rx_frame, .len = rx_frame_size}};
    return compute_inet_checksum(frame_slice, 4);
}

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

    if (compute_udp_checksum(
            mdata->ip_mdata->src_ip,
            mdata->ip_mdata->dest_ip,
            rx_frame,
            rx_frame_size)
        != 0)
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

int8_t udp_buid_frame(
    struct udp*             self,
    struct udp_rx_metadata* mdata,
    uint8_t*                tx_frame,
    uint16_t                tx_frame_size)
{
    return 0;
}

/* ========================================================================== */
