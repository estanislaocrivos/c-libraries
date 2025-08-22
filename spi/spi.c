#include "spi.h"

/* ============================================================================================== */

#include <string.h>

/* ============================================================================================== */

int8_t spi_create(spi_t* self, const spi_drivers_t* drivers)
{
    if (self == NULL || drivers == NULL || drivers->initialize == NULL
        || drivers->write_word == NULL || drivers->read_word == NULL
        || drivers->set_rx_callback == NULL || drivers->clear_buffers == NULL)
    {
        return -EFAULT;
    }
    memset(self, 0, sizeof(spi_t));
    self->initialize      = drivers->initialize;
    self->write_word      = drivers->write_word;
    self->read_word       = drivers->read_word;
    self->set_rx_callback = drivers->set_rx_callback;
    self->clear_buffers   = drivers->clear_buffers;
    return 0;
}

/* ============================================================================================== */
