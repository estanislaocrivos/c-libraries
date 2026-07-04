#include "../inc/icmp.h"

#include "../../inc/errno.h"
#include "../inc/utils.h"

#include <stddef.h>
#include <string.h>

/* ========================================================================== */

#define ICMP_TYPE_OFST     0
#define ICMP_CODE_OFST     1
#define ICMP_CHECKSUM_OFST 2
#define ICMP_ID_OFST       4
#define ICMP_SEQ_OFST      6
#define ICMP_DATA_OFST     8

/* ========================================================================== */

int8_t icmp_process_frame(
    const struct icmp*       self,
    const uint8_t*           rx_frame,
    uint16_t                 rx_frame_size,
    struct icmp_rx_metadata* mdata)
{
    if (self == NULL || rx_frame == NULL || mdata == NULL)
    {
        return -EFAULT;
    }

    if (rx_frame_size < ICMP_HDR_SIZE)
    {
        return -EINVAL;
    }

    if (compute_inet_checksum(rx_frame, rx_frame_size) != 0)
    {
        return -EINVAL;
    }

    mdata->type = rx_frame[ICMP_TYPE_OFST];
    mdata->code = rx_frame[ICMP_CODE_OFST];
    mdata->id   = ((uint16_t)rx_frame[ICMP_ID_OFST] << 8)
                | rx_frame[ICMP_ID_OFST + 1];
    mdata->seq_num = ((uint16_t)rx_frame[ICMP_SEQ_OFST] << 8)
                     | rx_frame[ICMP_SEQ_OFST + 1];
    mdata->payload      = rx_frame + ICMP_DATA_OFST;
    mdata->payload_size = rx_frame_size - ICMP_HDR_SIZE;

    return 0;
}

/* ========================================================================== */

int8_t icmp_build_frame(
    const struct icmp*       self,
    struct icmp_tx_metadata* mdata,
    uint8_t*                 tx_frame,
    uint16_t                 tx_frame_size)
{
    if (self == NULL || mdata == NULL || tx_frame == NULL)
    {
        return -EFAULT;
    }

    uint16_t required = ICMP_HDR_SIZE + mdata->payload_size;
    if (tx_frame_size < required)
    {
        return -EINVAL;
    }

    tx_frame[ICMP_TYPE_OFST]         = mdata->type;
    tx_frame[ICMP_CODE_OFST]         = mdata->code;
    tx_frame[ICMP_CHECKSUM_OFST]     = 0;
    tx_frame[ICMP_CHECKSUM_OFST + 1] = 0;
    tx_frame[ICMP_ID_OFST]           = (uint8_t)(mdata->id >> 8);
    tx_frame[ICMP_ID_OFST + 1]       = (uint8_t)(mdata->id & 0xFF);
    tx_frame[ICMP_SEQ_OFST]          = (uint8_t)(mdata->seq_num >> 8);
    tx_frame[ICMP_SEQ_OFST + 1]      = (uint8_t)(mdata->seq_num & 0xFF);

    if (mdata->payload != NULL && mdata->payload_size > 0)
    {
        memcpy(tx_frame + ICMP_DATA_OFST, mdata->payload, mdata->payload_size);
    }

    uint16_t checksum            = compute_inet_checksum(tx_frame, required);
    tx_frame[ICMP_CHECKSUM_OFST] = (uint8_t)(checksum >> 8);
    tx_frame[ICMP_CHECKSUM_OFST + 1] = (uint8_t)(checksum & 0xFF);

    return 0;
}

/* ========================================================================== */
