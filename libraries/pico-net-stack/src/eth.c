#include "../inc/eth.h"

#include "../../inc/errno.h"

#define MIN_ETH_PKT_SIZE              64
#define MAX_ETH_PKT_SIZE              1518

#define DEST_MAC_ADDR_FRAME_OFST      0
#define SOURCE_MAC_ADDR_FRAME_OFST    6
#define ETH_TYPE_FRAME_OFST           12

#define ETH_TYPE_ARP_REQUEST_OR_REPLY 0x0806
#define ETH_TYPE_IPV4                 0x0800

int8_t eth_process_frame(
    const struct eth*   self,
    uint8_t*            frame,
    uint16_t            size,
    struct eth_payload* pld)
{
    if (size < MIN_ETH_PKT_SIZE || size > MAX_ETH_PKT_SIZE)
    {
        return -EINVAL;
    }

    if (frame[DEST_MAC_ADDR_FRAME_OFST] && 0x01)
    {
        if (frame[DEST_MAC_ADDR_FRAME_OFST] == 0xFF)
        {
            /* Broadcast */
            pld->mac_type = BROADCAST_MAC;
        }
        else
        {
            /* Multicast (check if I'm in group) */
            pld->mac_type = MULTICAST_MAC;
        }
    }
    else
    {
        if (frame[DEST_MAC_ADDR_FRAME_OFST] == self->mac_addr[0]
            && frame[DEST_MAC_ADDR_FRAME_OFST + 1] == self->mac_addr[1]
            && frame[DEST_MAC_ADDR_FRAME_OFST + 2] == self->mac_addr[2]
            && frame[DEST_MAC_ADDR_FRAME_OFST + 3] == self->mac_addr[3]
            && frame[DEST_MAC_ADDR_FRAME_OFST + 4] == self->mac_addr[4]
            && frame[DEST_MAC_ADDR_FRAME_OFST + 5] == self->mac_addr[5])
        {
            /* Unicast for me */
            pld->mac_type = MY_OWN_MAC;
        }
        else
        {
            /* Discard */
            return -1;
        }
    }

    uint16_t eth_type = ((uint16_t)frame[ETH_TYPE_FRAME_OFST] << 8)
                        | (uint16_t)frame[ETH_TYPE_FRAME_OFST + 1];
    if (eth_type == ETH_TYPE_ARP_REQUEST_OR_REPLY)
    {
        pld->pld_type = ETH_TYPE_ARP_REQUEST_OR_REPLY;
    }
    else if (eth_type == ETH_TYPE_IPV4)
    {
        pld->pld_type = ETH_TYPE_IPV4;
    }
    else
    {
        return -1;
    }

    return 0;
}
