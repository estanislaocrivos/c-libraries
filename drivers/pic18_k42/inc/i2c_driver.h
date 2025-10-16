#ifndef I2C_DRIVER_H
#define I2C_DRIVER_H

/* ============================================================================================== */

#include "config.h"
#include "../../libraries/embedded-hal/inc/i2c.h"

/* ============================================================================================== */

int8_t initialize(struct i2c_port* self);

/* ============================================================================================== */

int8_t write(struct i2c_port* self,
             uint8_t          reg_address,
             const uint8_t*   tx_payload,
             size_t           tx_payload_size);

/* ============================================================================================== */

int8_t i2c_read(struct i2c_port* self,
                uint8_t          reg_address,
                uint8_t*         rx_payload,
                size_t           rx_payload_size);

/* ============================================================================================== */

#endif  // I2C_DRIVER_H
