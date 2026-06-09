# PIC18-K42 Drivers

Drivers for the PIC18F-K42 microcontroller family.

**Note:** These drivers are expected to be used through the `embedded-hal` abstraction interfaces. Direct usage is possible but not recommended. All the drivers implement the interfaces defined in `libraries/c/embedded-hal/inc/embedded_hal.h`.

## Prerequisites

| Tool | Version | Purpose |
| ------ | --------- | --------- |
| [XC8](https://www.microchip.com/en-us/tools-resources/develop/mplab-xc-compilers/xc8) | ≥ 3.0 | Compiler |
| Target Microcontroller | PIC18F xxK42 family | Supported MCUs |

## Usage

Create a project where you will develop your application. A CMake-based project is recommended. The recommended structure is as follows:

```text
project/
├── CMakeLists.txt
├── cmake/
│   ├── boards/
│   ├── Modules/
│   └── toolchains/
├── external/
│   └── (this repository)
├── include/
│   ├── main.h
│   ├── pic18_pinout_config.h
│   └── ...
└── src/
    └── main.c
```

### Fundamental Includes

Your `main.h` file should include the `platform.h` header, which automatically includes all the drivers for the detected platform. It should also include the `embedded_hal.h` header to access the HAL interfaces:

```c
#include "platform.h"
#include "embedded_hal.h"
```

This allows for more flexibility if you are using a different microcontroller in the future. For example, the CMake toolchain for the PIC18-K42 should include the `platform.h` inside the `pic18_k42` folder, but if you switch to another microcontroller, you only need to change the toolchain, and the correct `platform.h` will be included automatically. The `embedded_hal.h` header provides access to the HAL interfaces, which are hardware-agnostic and must be implemented by the specific drivers.

### Pin Configuration

You must define your pin configuration in a file named `pic18_pinout_config.h` located in your project's `include/` directory. This file will be included by the drivers to determine the configuration of each pin. An example file is provided in `libraries/c/drivers/pic18_k42/inc/pic18_pinout_config_example.h`. An example configuration is shown below:

```c
#define GPIO_A0 PIN_DO /* Digital Output */
#define GPIO_A1 PIN_DI /* Digital Input */
#define GPIO_A2 PIN_NC /* Not Connected */
#define GPIO_A3 PIN_NC
#define GPIO_A4 PIN_NC
#define GPIO_A5 PIN_NC
#define GPIO_A6 PIN_NC
#define GPIO_A7 PIN_NC

#define GPIO_B0 PIN_DO /* Digital Output */
#define GPIO_B1 PIN_DI /* Digital Input */
#define GPIO_B2 PIN_NC
#define GPIO_B3 PIN_NC

#define GPIO_A0_OUT_PPS_SETTING I2C2_SDA_OUT_PPS_SETTING /* I2C2 SDA Output */
#define GPIO_B0_OUT_PPS_SETTING I2C2_SCL_OUT_PPS_SETTING /* I2C2 SCL Output */

#define GPIO_B0_IN_PPS_REG SPI1SCKPPS_IN_PPS_REG /* SPI1 SCK Input */
#define GPIO_B1_IN_PPS_REG SPI1SDIPPS_IN_PPS_REG /* SPI1 SDI Input */
...
```

These macros are processed by the `pic18_pinout.h` header to configure the pins accordingly, and used by the private functions `pic18_pps_in_init_pinout()` and `pic18_pps_out_init_pinout()` to set up the PPS registers. All these functions are called inside the public function `pic18_mcu_initialize()`.

Although this file is hardware-specific, your compilation toolchain should choose to include it or not on the build process depending on the target microcontroller. This way, you can maintain a single codebase for multiple microcontrollers by simply changing the toolchain configuration.

### Oscillator Configuration

The main oscillator configuration can be defined through the `_XTAL_FREQ` macro, which should be defined in your project before including any driver headers. This macro indicates the frequency of the external crystal oscillator connected to the microcontroller. The `pic18_mcu_initialize()` function will use this macro to configure the internal oscillator accordingly.

### How to Use the Drivers

The main idea behind using an abstraction layer like `embedded-hal` is to program against interfaces rather than specific implementations. This allows for greater portability and flexibility in your code. To use the drivers of a specific platform, you should access them through the `embedded-hal` interfaces. The `platform.h` header defines a set of `_OPS` macros (which define the specific driver operations for each peripheral) to help you create instances of the various peripherals available on the platform. These macros can be used in conjunction with the `_DEFINE` macros provided by `embedded-hal` to create and initialize peripheral instances. For example, here is a basic main function that initializes the microcontroller and various peripherals using the HAL interfaces:

```c
int main(void)
{
    INTERRUPT_DEFINE(interrupt_manager, PLATFORM_INTERRUPT_OPS); /* See embedded-hal docs for details on the _DEFINE macros */
    interrupt_manager.ops->enable_high_priority(true);
    interrupt_manager.ops->enable_low_priority(true);
    interrupt_manager.ops->atomic_lock(); /* Start with interrupts disabled */

    MCU_DEFINE(mcu, PLATFORM_MCU_OPS);
    mcu.ops->initialize(&mcu); /* If compiled for pic18, this function should call pic18_mcu_initialize() internally */

    GPIO_DEFINE(led_gpio, PLATFORM_GPIO_OPS, 12, GPIO_DIGITAL, GPIO_OUTPUT);
    led_gpio.ops->initialize(&led_gpio);

    SERIAL_DEFINE(serial_log, PLATFORM_SERIAL2_OPS, 9600, 8, 'N', 1);
    serial_log.ops->initialize(&serial_log);

    SERIAL_DEFINE(serial, PLATFORM_SERIAL1_OPS, 9600, 8, 'N', 1);
    serial.ops->initialize(&serial);
    serial.ops->set_rx_interrupt_priority(&serial, 0);
    serial.ops->enable_rx_interrupt(&serial, true);
}
```

As you can see, the code is completely hardware-agnostic. If you decide to switch to another microcontroller in the future, you only need to change the toolchain configuration, and the correct drivers will be used automatically without modifying your application code.
