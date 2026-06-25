#include "../inc/arp.h"

#include "../../inc/errno.h"

#include <stddef.h>
#include <stdint.h>

/* ========================================================================== */

#define MIN_ARP_FRAME_SIZE            42
#define ETH_TYPE_ARP_REQUEST_OR_REPLY 0x0806
#define OP_TYPE_ARP_REQUEST           0x01

#define DEST_MAC_ADDR_FRAME_OFST      0
#define SOURCE_MAC_ADDR_FRAME_OFST    6
#define ETH_TYPE_FRAME_OFST           12
#define HARD_TYPE_FRAME_OFST          14
#define PROT_TYPE_FRAME_OFST          16
#define HARD_SIZE_FRAME_OFST          18
#define PROT_SIZE_FRAME_OFST          19
#define OP_FRAME_OFST                 20
#define SENDER_MAC_ADDR_FRAME_OFST    22
#define SENDER_IP_ADDR_FRAME_OFST     28
#define TARGET_MAC_ADDR_FRAME_OFST    32
#define TARGET_IP_ADDR_FRAME_OFST     38

int8_t arp_is_request_for_me(
    const struct arp* self, uint8_t* frame, uint8_t size, bool* state)
{
    if (self == NULL || frame == NULL)
    {
        return -EFAULT;
    }
    if (size < MIN_ARP_FRAME_SIZE)
    {
        return -EINVAL;
    }

    uint16_t eth_type = ((uint16_t)frame[ETH_TYPE_FRAME_OFST] << 8)
                        | (uint16_t)frame[ETH_TYPE_FRAME_OFST + 1];
    if (eth_type != ETH_TYPE_ARP_REQUEST_OR_REPLY)
    {
        return -EINVAL;
    }

    uint16_t op_type = ((uint16_t)frame[OP_FRAME_OFST] << 8)
                       | (uint16_t)frame[OP_FRAME_OFST + 1];
    if (op_type != OP_TYPE_ARP_REQUEST)
    {
        return -EINVAL;
    }

    *state
        = (frame[TARGET_IP_ADDR_FRAME_OFST] == self->ip_addr[0]
           && frame[TARGET_IP_ADDR_FRAME_OFST + 1] == self->ip_addr[1]
           && frame[TARGET_IP_ADDR_FRAME_OFST + 2] == self->ip_addr[2]
           && frame[TARGET_IP_ADDR_FRAME_OFST + 3] == self->ip_addr[3]);
    return 0;
}

/* ========================================================================== */

int8_t arp_reply_request(const struct arp* self, uint8_t* frame, uint8_t size)
{
    if (self == NULL || frame == NULL)
    {
        return -EFAULT;
    }
    if (size < MIN_ARP_FRAME_SIZE)
    {
        return -EINVAL;
    }

    frame[DEST_MAC_ADDR_FRAME_OFST]     = frame[SENDER_MAC_ADDR_FRAME_OFST];
    frame[DEST_MAC_ADDR_FRAME_OFST + 1] = frame[SENDER_MAC_ADDR_FRAME_OFST + 1];
    frame[DEST_MAC_ADDR_FRAME_OFST + 2] = frame[SENDER_MAC_ADDR_FRAME_OFST + 2];
    frame[DEST_MAC_ADDR_FRAME_OFST + 3] = frame[SENDER_MAC_ADDR_FRAME_OFST + 3];
    frame[DEST_MAC_ADDR_FRAME_OFST + 4] = frame[SENDER_MAC_ADDR_FRAME_OFST + 4];
    frame[DEST_MAC_ADDR_FRAME_OFST + 5] = frame[SENDER_MAC_ADDR_FRAME_OFST + 5];

    frame[SOURCE_MAC_ADDR_FRAME_OFST]     = self->mac_addr[0];
    frame[SOURCE_MAC_ADDR_FRAME_OFST + 1] = self->mac_addr[1];
    frame[SOURCE_MAC_ADDR_FRAME_OFST + 2] = self->mac_addr[2];
    frame[SOURCE_MAC_ADDR_FRAME_OFST + 3] = self->mac_addr[3];
    frame[SOURCE_MAC_ADDR_FRAME_OFST + 4] = self->mac_addr[4];
    frame[SOURCE_MAC_ADDR_FRAME_OFST + 5] = self->mac_addr[5];

    frame[OP_FRAME_OFST]     = 0;
    frame[OP_FRAME_OFST + 1] = 2;

    frame[TARGET_MAC_ADDR_FRAME_OFST]     = frame[DEST_MAC_ADDR_FRAME_OFST];
    frame[TARGET_MAC_ADDR_FRAME_OFST + 1] = frame[DEST_MAC_ADDR_FRAME_OFST + 1];
    frame[TARGET_MAC_ADDR_FRAME_OFST + 2] = frame[DEST_MAC_ADDR_FRAME_OFST + 2];
    frame[TARGET_MAC_ADDR_FRAME_OFST + 3] = frame[DEST_MAC_ADDR_FRAME_OFST + 3];
    frame[TARGET_MAC_ADDR_FRAME_OFST + 4] = frame[DEST_MAC_ADDR_FRAME_OFST + 4];
    frame[TARGET_MAC_ADDR_FRAME_OFST + 5] = frame[DEST_MAC_ADDR_FRAME_OFST + 5];

    frame[SENDER_MAC_ADDR_FRAME_OFST] = frame[SOURCE_MAC_ADDR_FRAME_OFST];
    frame[SENDER_MAC_ADDR_FRAME_OFST + 1]
        = frame[SOURCE_MAC_ADDR_FRAME_OFST + 1];
    frame[SENDER_MAC_ADDR_FRAME_OFST + 2]
        = frame[SOURCE_MAC_ADDR_FRAME_OFST + 2];
    frame[SENDER_MAC_ADDR_FRAME_OFST + 3]
        = frame[SOURCE_MAC_ADDR_FRAME_OFST + 3];
    frame[SENDER_MAC_ADDR_FRAME_OFST + 4]
        = frame[SOURCE_MAC_ADDR_FRAME_OFST + 4];
    frame[SENDER_MAC_ADDR_FRAME_OFST + 5]
        = frame[SOURCE_MAC_ADDR_FRAME_OFST + 5];

    frame[TARGET_IP_ADDR_FRAME_OFST]     = frame[SENDER_IP_ADDR_FRAME_OFST];
    frame[TARGET_IP_ADDR_FRAME_OFST + 1] = frame[SENDER_IP_ADDR_FRAME_OFST + 1];
    frame[TARGET_IP_ADDR_FRAME_OFST + 2] = frame[SENDER_IP_ADDR_FRAME_OFST + 2];
    frame[TARGET_IP_ADDR_FRAME_OFST + 3] = frame[SENDER_IP_ADDR_FRAME_OFST + 3];

    frame[SENDER_IP_ADDR_FRAME_OFST]     = self->ip_addr[0];
    frame[SENDER_IP_ADDR_FRAME_OFST + 1] = self->ip_addr[1];
    frame[SENDER_IP_ADDR_FRAME_OFST + 2] = self->ip_addr[2];
    frame[SENDER_IP_ADDR_FRAME_OFST + 3] = self->ip_addr[3];

    return 0;
}

/* ========================================================================== */
