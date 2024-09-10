# Small Memory Manager

A small memory manager written in pure C, designed for embedded systems or environments with limited resources. This project is simple to integrate, with only one `.h` and one `.c` file, and does not rely on any external libraries.

## Features

- **Pure C Implementation**: No external dependencies, just a single `.h` and `.c` file.
- **Memory Size Range**: Supports memory management from 16 bytes up to 2MB.
- **Minimum Allocation Size**: Allows for 4-byte minimum memory allocation.
- **Custom Memory Read/Write Interfaces**: Easily extendable to manage memory over I2C, SPI, or any other communication protocols.
- **Non-Volatile Memory Management**: Supports reconstructing memory state after power loss, ideal for managing non-volatile memories.

## Getting Started

### Requirements

- A C compiler (e.g., GCC)
- An embedded environment with a maximum of 2MB memory to manage.

### Integration

1. Copy the `smm.h` and `smm.c` files into your project.
2. Include the header file in your source code:
   ```c
   #include "smm.h"

### Example Usage
```
#include "smm.h"

// Use custom read/write interface
smm_mem_opt read;
smm_mem_opt write;

// Initialize memory manager for a 2MB block
struct smm_mem_info mem_mgr = {
    .base_addr = YOUR_MEMORY_START_ADDRESS;
    .size = 2 * 1024 * 1024;
    .read = read;
    .write = write;
};
smm_memory_create(&mem_mgr);

// Allocate 128 bytes
void* ptr = smm_malloc(&mem_mgr, 128);

// Deallocate memory
smm_free(&mem_mgr, ptr);
```
### Customization
Custom Read/Write Functions: The memory manager allows you to set custom memory read and write functions, which makes it suitable for managing external memory connected through IIC or SPI.

```
int read(u8 *dst_addr, u8 *src_addr, u32 size)
{
    // Implement your read operation here
}

int write(u8 *dst_addr, u8 *src_addr, u32 size)
{
    // Implement your write operation here
}
```
Rebuild After Power Loss: The memory manager provides functionality to rebuild memory management structures in volatile memories after a power failure.

## License
This project is licensed under the BSD-3-Clause License - see the LICENSE file for details.

