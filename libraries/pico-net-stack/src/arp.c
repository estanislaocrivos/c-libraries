#include "../inc/arp.h"

#include "../../inc/errno.h"

#include <stddef.h>
#include <stdint.h>
#include <string.h>

/* ========================================================================== */

#define MIN_ARP_FRAME_SIZE         42

#define ARP_REQUEST_OPCODE_VAL     ((uint16_t)0x0001)
#define ARP_REPLY_OPCODE_VAL       ((uint16_t)0x0002)
#define ARP_REV_REQUEST_OPCODE_VAL ((uint16_t)0x0003)
#define ARP_REV_REPLY_OPCODE_VAL   ((uint16_t)0x0004)

#define HARD_TYPE_VAL              ((uint16_t)0x0001)
#define PROT_TYPE_VAL              ((uint16_t)0x0800)
#define HARD_SIZE_VAL              6
#define PROT_SIZE_VAL              4

#define HARD_TYPE_FRAME_OFST       14
#define PROT_TYPE_FRAME_OFST       16
#define HARD_SIZE_FRAME_OFST       18
#define PROT_SIZE_FRAME_OFST       19
#define OPCODE_FRAME_OFST          20
#define SRC_MAC_ADDR_FRAME_OFST    22
#define SRC_IP_ADDR_FRAME_OFST     28
#define DEST_MAC_ADDR_FRAME_OFST   32
#define DEST_IP_ADDR_FRAME_OFST    38

/* ========================================================================== */

int8_t arp_process_frame(
    const struct arp*       self,
    const uint8_t*          rx_frame,
    uint8_t                 rx_frame_size,
    struct arp_rx_metadata* mdata)

{
    if (self == NULL || rx_frame == NULL)
    {
        return -EFAULT;
    }
    if (rx_frame_size < MIN_ARP_FRAME_SIZE)
    {
        return -EINVAL;
    }

    uint16_t op_type = ((uint16_t)rx_frame[OPCODE_FRAME_OFST] << 8)
                       | (uint16_t)rx_frame[OPCODE_FRAME_OFST + 1];
    switch (op_type)
    {
        case ARP_REQUEST_OPCODE_VAL:
        {
            mdata->op_type = ARP_REQUEST;
            break;
        }
        case ARP_REPLY_OPCODE_VAL:
        {
            mdata->op_type = ARP_REPLY;
            break;
        }
        case ARP_REV_REQUEST_OPCODE_VAL:
        {
            mdata->op_type = ARP_REV_REQUEST;
            break;
        }
        case ARP_REV_REPLY_OPCODE_VAL:
        {
            mdata->op_type = ARP_REV_REPLY;
            break;
        }
        default:
        {
            return -EINVAL;
        }
    }

    memcpy(mdata->src_ip_addr, rx_frame + SRC_IP_ADDR_FRAME_OFST, 4);
    memcpy(mdata->src_mac_addr, rx_frame + SRC_MAC_ADDR_FRAME_OFST, 6);
    return 0;
}

/* ========================================================================== */

int8_t arp_build_frame(
    const struct arp*       self,
    struct arp_tx_metadata* mdata,
    uint8_t*                tx_frame,
    uint8_t                 tx_frame_size)
{
    if (self == NULL || mdata == NULL || tx_frame == NULL)
    {
        return -EFAULT;
    }
    if (tx_frame_size < MIN_ARP_FRAME_SIZE)
    {
        return -EINVAL;
    }

    tx_frame[HARD_TYPE_FRAME_OFST]     = (uint8_t)(HARD_TYPE_VAL >> 8);
    tx_frame[HARD_TYPE_FRAME_OFST + 1] = (uint8_t)(HARD_TYPE_VAL);
    tx_frame[PROT_TYPE_FRAME_OFST]     = (uint8_t)(PROT_TYPE_VAL >> 8);
    tx_frame[PROT_TYPE_FRAME_OFST + 1] = (uint8_t)(PROT_TYPE_VAL);
    tx_frame[HARD_SIZE_FRAME_OFST]     = (uint8_t)(HARD_SIZE_VAL);
    tx_frame[PROT_SIZE_FRAME_OFST]     = (uint8_t)(PROT_SIZE_VAL);

    switch (mdata->op_type)
    {
        case ARP_REQUEST:
        {
            tx_frame[OPCODE_FRAME_OFST]
                = (uint8_t)(ARP_REQUEST_OPCODE_VAL >> 8);
            tx_frame[OPCODE_FRAME_OFST + 1] = (uint8_t)(ARP_REQUEST_OPCODE_VAL);
            break;
        }
        case ARP_REPLY:
        {
            tx_frame[OPCODE_FRAME_OFST] = (uint8_t)(ARP_REPLY_OPCODE_VAL >> 8);
            tx_frame[OPCODE_FRAME_OFST + 1] = (uint8_t)(ARP_REPLY_OPCODE_VAL);
            break;
        }
        case ARP_REV_REQUEST:
        {
            tx_frame[OPCODE_FRAME_OFST]
                = (uint8_t)(ARP_REV_REQUEST_OPCODE_VAL >> 8);
            tx_frame[OPCODE_FRAME_OFST + 1]
                = (uint8_t)(ARP_REV_REQUEST_OPCODE_VAL);
            break;
        }
        case ARP_REV_REPLY:
        {
            tx_frame[OPCODE_FRAME_OFST]
                = (uint8_t)(ARP_REV_REPLY_OPCODE_VAL >> 8);
            tx_frame[OPCODE_FRAME_OFST + 1]
                = (uint8_t)(ARP_REV_REPLY_OPCODE_VAL);
            break;
        }
        default:
        {
            return -EINVAL;
        }
    }

    memcpy(tx_frame + DEST_MAC_ADDR_FRAME_OFST, mdata->dest_mac_addr, 6);
    memcpy(tx_frame + DEST_IP_ADDR_FRAME_OFST, mdata->dest_ip_addr, 4);
    memcpy(tx_frame + SRC_MAC_ADDR_FRAME_OFST, self->mac_addr, 6);
    memcpy(tx_frame + SRC_IP_ADDR_FRAME_OFST, self->ip_addr, 4);
    return 0;
}

/* ========================================================================== */
