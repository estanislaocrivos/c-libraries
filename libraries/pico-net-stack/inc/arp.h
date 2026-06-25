#ifndef ARP_H
#define ARP_H

/* ========================================================================== */

#include <stdbool.h>
#include <stdint.h>

/* ========================================================================== */

struct arp
{
    uint8_t ip_addr[4];
    uint8_t mac_addr[6];
};

int8_t arp_is_request_for_me(
    const struct arp* self, uint8_t* frame, uint8_t size, bool* state);

int8_t arp_reply_request(const struct arp* self, uint8_t* frame, uint8_t size);

/* ========================================================================== */

#endif /* ARP_H */
