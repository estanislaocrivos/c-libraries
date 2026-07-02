#include "../inc/icmp.h"

#include <stdbool.h>
#include <stdint.h>

struct icmp
{
    bool nothing;
};

struct icmp_rx_metadata
{
    uint8_t  type;
    uint8_t  code;
    uint16_t checksum;
    uint16_t id;
    uint16_t seq_num;
};

struct icmp_tx_metadata
{
    uint8_t  type;
    uint8_t  code;
    uint16_t checksum;
    uint16_t id;
    uint16_t seq_num;
};

int8_t icmp_process_frame(
    const struct icmp*       self,
    const uint8_t*           rx_frame,
    uint8_t                  rx_frame_size,
    struct icmp_rx_metadata* mdata);

int8_t icmp_build_frame(
    const struct icmp*       self,
    struct icmp_tx_metadata* mdata,
    const uint8_t*           tx_frame,
    uint8_t                  tx_frame_size);
