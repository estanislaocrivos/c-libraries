#include "../inc/udp.h"

/* ========================================================================== */

#include "../../inc/errno.h"
#include "../inc/utils.h"

#include <stddef.h>
#include <string.h>

/* ========================================================================== */

/* UDP_HEADER_SIZE comes from udp.h (const, shared with the caller's offset
 * arithmetic in main.c). IP_PLD_PROT_UDP_VAL mirrors ip.c's macro of the same
 * name: kept as a macro there because it's used as a switch/case label, so
 * it stays a macro here too for consistency even though this use doesn't
 * require it. */
#define IP_PLD_PROT_UDP_VAL (uint8_t)17

static const uint8_t UDP_SRC_PORT_FRAME_OFST  = 0; /* 2 bytes */
static const uint8_t UDP_DEST_PORT_FRAME_OFST = 2; /* 2 bytes */
static const uint8_t UDP_LENGTH_FRAME_OFST    = 4; /* 2 bytes */
static const uint8_t UDP_CHECKSUM_FRAME_OFST  = 6; /* 2 bytes */
/* Payload size = UDP_LENGTH_FRAME_OFST - UDP_HEADER_SIZE bytes */
static const uint8_t UDP_PAYLOAD_FRAME_OFST = 8;

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

    mdata->src_port_num = (uint16_t)((rx_frame[UDP_SRC_PORT_FRAME_OFST]) << 8)
                          | (rx_frame[UDP_SRC_PORT_FRAME_OFST + 1]);
    mdata->dest_port_num = (uint16_t)((rx_frame[UDP_DEST_PORT_FRAME_OFST]) << 8)
                           | (rx_frame[UDP_DEST_PORT_FRAME_OFST + 1]);
    mdata->payload      = rx_frame + UDP_PAYLOAD_FRAME_OFST;
    mdata->payload_size = rx_frame[UDP_LENGTH_FRAME_OFST] - UDP_HEADER_SIZE;
    return 0;
}

/* ========================================================================== */

int8_t udp_build_frame(
    struct udp*             self,
    struct udp_tx_metadata* mdata,
    uint8_t*                tx_frame,
    uint16_t*               tx_frame_size)
{
    if (self == NULL || mdata == NULL || tx_frame == NULL
        || tx_frame_size == NULL)
    {
        return -EFAULT;
    }

    uint16_t udp_len = UDP_HEADER_SIZE + mdata->payload_size;

    tx_frame[UDP_SRC_PORT_FRAME_OFST]     = (uint8_t)(mdata->src_port_num >> 8);
    tx_frame[UDP_SRC_PORT_FRAME_OFST + 1] = (uint8_t)(mdata->src_port_num);
    tx_frame[UDP_DEST_PORT_FRAME_OFST] = (uint8_t)(mdata->dest_port_num >> 8);
    tx_frame[UDP_DEST_PORT_FRAME_OFST + 1] = (uint8_t)(mdata->dest_port_num);
    tx_frame[UDP_LENGTH_FRAME_OFST]        = (uint8_t)(udp_len >> 8);
    tx_frame[UDP_LENGTH_FRAME_OFST + 1]    = (uint8_t)(udp_len);
    tx_frame[UDP_CHECKSUM_FRAME_OFST]      = 0;
    tx_frame[UDP_CHECKSUM_FRAME_OFST + 1]  = 0;

    uint16_t checksum = compute_udp_checksum(
        mdata->ip_mdata->src_ip, mdata->ip_mdata->dest_ip, tx_frame, udp_len);
    tx_frame[UDP_CHECKSUM_FRAME_OFST]     = (uint8_t)(checksum >> 8);
    tx_frame[UDP_CHECKSUM_FRAME_OFST + 1] = (uint8_t)(checksum);

    *tx_frame_size = udp_len;
    return 0;
}

/* ========================================================================== */
