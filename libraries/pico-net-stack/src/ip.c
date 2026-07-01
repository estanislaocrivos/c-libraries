#include "../inc/ip.h"

#include "../../inc/errno.h"

#include <stddef.h>
#include <stdint.h>
#include <string.h>

/* ========================================================================== */

#define IP_VERSION_IHL_FRAME_OFST 0 /* Version (bits 7-4) + IHL (bits 3-0) */
#define IP_DSCP_ECN_FRAME_OFST    1 /* DSCP (bits 7-2) + ECN (bits 1-0) */
#define IP_TOTAL_LEN_FRAME_OFST   2 /* 2 bytes */
#define IP_ID_FRAME_OFST          4 /* 2 bytes */
#define IP_FLAGS_FRAG_FRAME_OFST \
    6 /* Flags (bits 7-5) + Fragment offset (bits 4-0 + next byte) */
#define IP_TTL_FRAME_OFST      8
#define IP_PROT_FRAME_OFST     9
#define IP_CHECKSUM_FRAME_OFST 10 /* 2 bytes */
#define IP_SRC_IP_FRAME_OFST   12 /* 4 bytes */
#define IP_DEST_IP_FRAME_OFST  16 /* 4 bytes */

#define IP_MIN_HDR_SIZE        20 /* IHL=5, no options */

#define IP_VERSION_MASK        0xF0
#define IP_IHL_MASK            0x0F

#define IP_VER_4_VAL           4
#define IP_VER_6_VAL           6

#define IP_PLD_PROT_UDP_VAL    17
#define IP_PLD_PROT_TCP_VAL    6
#define IP_PLD_PROT_ICMP_VAL   1

/* ========================================================================== */

int8_t ip_process_frame(
    const struct ip*       self,
    const uint8_t*         rx_frame,
    uint16_t               rx_frame_size,
    struct ip_rx_metadata* mdata)
{
    if (self == NULL || rx_frame == NULL || mdata == NULL)
    {
        return -EFAULT;
    }

    if (rx_frame_size < IP_MIN_HDR_SIZE)
    {
        return -EINVAL;
    }

    uint8_t ip_ver = (rx_frame[IP_VERSION_IHL_FRAME_OFST] & IP_VERSION_MASK)
                     >> 4;
    switch (ip_ver)
    {
        case IP_VER_4_VAL:
        {
            mdata->version = IP_VER_4;
            break;
        }
        case IP_VER_6_VAL:
        {
            mdata->version = IP_VER_6;
            break;
        }
        default:
        {
            mdata->version = IP_VER_UNKNOWN;
            break;
        }
    }

    uint8_t prot = rx_frame[IP_PROT_FRAME_OFST];
    switch (prot)
    {
        case IP_PLD_PROT_UDP_VAL:
        {
            mdata->pld_prot_type = IP_PLD_UDP;
            break;
        }
        case IP_PLD_PROT_TCP_VAL:
        {
            mdata->pld_prot_type = IP_PLD_TCP;
            break;
        }
        case IP_PLD_PROT_ICMP_VAL:
        {
            mdata->pld_prot_type = IP_PLD_ICMP;
            break;
        }
        default:
        {
            mdata->pld_prot_type = IP_PLD_UNKNOWN;
            break;
        }
    }

    uint8_t ihl    = rx_frame[IP_VERSION_IHL_FRAME_OFST] & IP_IHL_MASK;
    mdata->payload = rx_frame + ihl * 4;

    uint16_t tot_len = ((uint16_t)rx_frame[IP_TOTAL_LEN_FRAME_OFST] << 8)
                       | (uint16_t)rx_frame[IP_TOTAL_LEN_FRAME_OFST + 1];
    mdata->payload_size = tot_len - 4 * ihl;

    memcpy(mdata->src_ip, rx_frame + IP_SRC_IP_FRAME_OFST, 4);
    memcpy(mdata->dest_ip, rx_frame + IP_DEST_IP_FRAME_OFST, 4);

    return 0;
}

/* ========================================================================== */

bool ip_is_pkt_for_me(const struct ip* self, const struct ip_rx_metadata* mdata)
{
    if (self == NULL || mdata == NULL)
    {
        return false;
    }

    return !memcmp(self->ip_addr, mdata->dest_ip, 4);
}

/* ========================================================================== */
