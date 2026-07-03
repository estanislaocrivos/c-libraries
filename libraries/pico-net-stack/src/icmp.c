#include "../inc/icmp.h"

#include "../../inc/errno.h"
#include "../inc/utils.h"

int8_t icmp_process_frame(
    const struct icmp*       self,
    const uint8_t*           rx_frame,
    uint8_t                  rx_frame_size,
    struct icmp_rx_metadata* mdata)
{
    if (compute_inet_checksum(rx_frame, rx_frame_size) != 0)
    {
        return -EINVAL;
    }

    mdata->type         = rx_frame[0];
    mdata->code         = rx_frame[1];
    mdata->id           = rx_frame[4];
    mdata->seq_num      = rx_frame[6];
    mdata->payload      = rx_frame + 8;
    mdata->payload_size = rx_frame_size - 8;

    return 0;
}

int8_t icmp_build_frame(
    const struct icmp*       self,
    struct icmp_tx_metadata* mdata,
    const uint8_t*           tx_frame,
    uint8_t                  tx_frame_size)
{
    return 0;
}
