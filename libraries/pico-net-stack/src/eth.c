#include "../inc/eth.h"

#include "../../inc/errno.h"

#include <stddef.h>

/* ========================================================================== */

#define MIN_ETH_PKT_SIZE              64
#define MAX_ETH_PKT_SIZE              1518

#define DEST_MAC_ADDR_FRAME_OFST      0
#define SOURCE_MAC_ADDR_FRAME_OFST    6
#define ETH_TYPE_FRAME_OFST           12

#define ETH_TYPE_ARP_REQUEST_OR_REPLY 0x0806
#define ETH_TYPE_IPV4                 0x0800
#define ETH_TYPE_IPV6                 0x08DD

/* ========================================================================== */

int8_t eth_process_frame(
    const struct eth*    self,
    const uint8_t*       rx_frame,
    uint16_t             rx_frame_size,
    struct eth_metadata* mdata)
{
    if (self == NULL)
    {
        return -EFAULT;
    }
    if (rx_frame_size < MIN_ETH_PKT_SIZE || rx_frame_size > MAX_ETH_PKT_SIZE)
    {
        return -EINVAL;
    }

    if (rx_frame[DEST_MAC_ADDR_FRAME_OFST] & 0x01)
    {
        if (rx_frame[DEST_MAC_ADDR_FRAME_OFST] == 0xFF
            && rx_frame[DEST_MAC_ADDR_FRAME_OFST + 1] == 0xFF
            && rx_frame[DEST_MAC_ADDR_FRAME_OFST + 2] == 0xFF
            && rx_frame[DEST_MAC_ADDR_FRAME_OFST + 3] == 0xFF
            && rx_frame[DEST_MAC_ADDR_FRAME_OFST + 4] == 0xFF
            && rx_frame[DEST_MAC_ADDR_FRAME_OFST + 5] == 0xFF)
        {
            mdata->mac_type = ETH_MAC_BROADCAST;
        }
        else
        {
            mdata->mac_type = ETH_MAC_MULTICAST;
        }
    }
    else
    {
        if (rx_frame[DEST_MAC_ADDR_FRAME_OFST] == self->mac_addr[0]
            && rx_frame[DEST_MAC_ADDR_FRAME_OFST + 1] == self->mac_addr[1]
            && rx_frame[DEST_MAC_ADDR_FRAME_OFST + 2] == self->mac_addr[2]
            && rx_frame[DEST_MAC_ADDR_FRAME_OFST + 3] == self->mac_addr[3]
            && rx_frame[DEST_MAC_ADDR_FRAME_OFST + 4] == self->mac_addr[4]
            && rx_frame[DEST_MAC_ADDR_FRAME_OFST + 5] == self->mac_addr[5])
        {
            mdata->mac_type = ETH_MAC_MY_OWN;
        }
        else
        {
            mdata->mac_type = ETH_MAC_UNKNOWN;
        }
    }

    uint16_t eth_type = ((uint16_t)rx_frame[ETH_TYPE_FRAME_OFST] << 8)
                        | (uint16_t)rx_frame[ETH_TYPE_FRAME_OFST + 1];
    if (eth_type == ETH_TYPE_ARP_REQUEST_OR_REPLY)
    {
        mdata->payload_type = ETH_PLD_ARP;
    }
    else if (eth_type == ETH_TYPE_IPV4)
    {
        mdata->payload_type = ETH_PLD_IPV4;
    }
    else if (eth_type == ETH_TYPE_IPV6)
    {
        mdata->payload_type = ETH_PLD_IPV6;
    }
    else
    {
        mdata->payload_type = ETH_PLD_UNKNOWN;
    }

    return 0;
}

/* ========================================================================== */
