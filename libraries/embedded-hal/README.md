# Embedded Hardware Abstraction Interfaces 🛠

Simple hardware abstraction interfaces for controlling embedded devices. These traits are designed to be implemented by device driver crates, and consumed by embedded application code. Based on the [embedded-hal](https://docs.rs/embedded-hal) Rust crate.

## Serial interface

### Example

```c
int main(void)
{
    const struct serial_ops my_serial_ops = {
        .initialize = my_serial_initialize,
        .transmit = my_serial_transmit,
        .receive = my_serial_receive,
        .set_rx_callback = my_serial_set_rx_callback,
        .enable_rx_interrupt = my_serial_enable_rx_interrupt,
        .clear_buffers = my_serial_clear_buffers,
    };
    struct serial_port my_serial;
    my_serial.ops = &my_serial_ops;
    my_serial.baud_rate = 9600;
    my_serial.rx_buffer = malloc(128);
    my_serial.rx_buffer_size = 128;
    my_serial.ops->initialize(&my_serial);
    // Other application code...
    return 0;
}
```
