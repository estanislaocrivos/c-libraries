# Buffer 📤

A simple C library for managing byte buffers with push and reset functionalities. It provides a straightforward interface to handle byte storage efficiently.

## Features

- Push bytes into the buffer.
- Reset the buffer to clear its contents.
- Check for buffer overflow conditions.

## Why did I create this library?

I created this library to have a lightweight and easy-to-use buffer management system for my C projects. It helps in scenarios where I need to handle byte streams, such as in communication protocols or data processing tasks. Although a library on its own might seem overkill for such a simple functionality, having a dedicated library for linear buffering avoids the creation of ad-hoc buffer management code (overflow checks, push and pop functions) in multiple projects, promoting code reuse and maintainability.

## Usage

Include the buffer header in your C file:

```c
#include "buffer.h"
```

Create and initialize a buffer:

```c
struct buffer buf;
uint8_t       buffer[256];
buf.buffer = buffer;
buf.size   = sizeof(buffer);
buffer_init(&buf);
```

Push bytes into the buffer:

```c
buffer_push(&buf, 0x01);
```

Reset the buffer when needed:

```c
buffer_reset(&buf);
```

## License

This library is licensed under the MIT License. See the [LICENSE](./LICENSE) file for more details.
