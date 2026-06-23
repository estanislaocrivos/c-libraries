#include "../inc/arp.h"

#include "../../inc/errno.h"

#include <stddef.h>

/* ========================================================================== */

int8_t arp_is_request_for_me(
    const struct arp* self, uint8_t* frame, uint16_t size, bool* state)
{
    if (self == NULL || frame == NULL)
    {
        return -EFAULT;
    }
    if (size < 42)
    {
        return -EINVAL;
    }

    uint16_t eth_type = ((uint16_t)frame[12] << 8) | (uint16_t)frame[13];
    if (eth_type != 0x0806) /* Check if ARP */
    {
        return -EINVAL;
    }

    uint16_t op_type = ((uint16_t)frame[20] << 8) | (uint16_t)frame[21];
    if (op_type != 0x0001) /* Check if request */
    {
        return -EINVAL;
    }

    *state
        = (frame[38] == self->ip_addr[3] && frame[39] == self->ip_addr[2]
           && frame[40] == self->ip_addr[1] && frame[41] == self->ip_addr[0]);
    return 0;
}

/* ========================================================================== */

int8_t arp_reply_request(const struct arp* self, uint8_t* frame, uint16_t size)
{
    if (self == NULL || frame == NULL)
    {
        return -EFAULT;
    }

    /* DST field */
    frame[0] = frame[22];
    frame[1] = frame[23];
    frame[2] = frame[24];
    frame[3] = frame[25];
    frame[4] = frame[26];
    frame[5] = frame[27];

    /* SRC field */
    frame[6]  = self->mac_addr[5];
    frame[7]  = self->mac_addr[4];
    frame[8]  = self->mac_addr[3];
    frame[9]  = self->mac_addr[2];
    frame[10] = self->mac_addr[1];
    frame[11] = self->mac_addr[0];

    frame[20] = 0;
    frame[21] = 2;

    frame[32] = frame[0];
    frame[33] = frame[1];
    frame[34] = frame[2];
    frame[35] = frame[3];
    frame[36] = frame[4];
    frame[37] = frame[5];

    frame[22] = frame[6];
    frame[23] = frame[7];
    frame[24] = frame[8];
    frame[25] = frame[9];
    frame[26] = frame[10];
    frame[27] = frame[11];

    frame[38] = frame[28];
    frame[39] = frame[29];
    frame[40] = frame[30];
    frame[41] = frame[31];

    frame[28] = self->ip_addr[3];
    frame[29] = self->ip_addr[2];
    frame[30] = self->ip_addr[1];
    frame[31] = self->ip_addr[0];

    return 0;
}

/* ========================================================================== */
