/* Date: 09.03.2025 */

#ifndef TASK_MMIO_MMIO_H
#define TASK_MMIO_MMIO_H

#include <stddef.h>
#include <stdint.h>

#define TASK_MMIO_PORT_START 0
#define TASK_MMIO_PORT_END   0xFFFF
#define TASK_MMIO_PORT_COUNT (TASK_MMIO_PORT_END - TASK_MMIO_PORT_START + 1)

int mmio_iorb(uintptr_t port, uint8_t *output);

int mmio_iorw(uintptr_t port, uint16_t *output);

int mmio_iord(uintptr_t port, uint32_t *output);

int mmio_iowb(uintptr_t port, uint8_t value);

int mmio_ioww(uintptr_t port, uint16_t value);

int mmio_iowd(uintptr_t port, uint32_t value);

int mmio_mmrb(uintptr_t mem, uint8_t *output);

int mmio_mmrw(uintptr_t mem, uint16_t *output);

int mmio_mmrd(uintptr_t mem, uint32_t *output);

int mmio_mmwb(uintptr_t mem, uint8_t value);

int mmio_mmww(uintptr_t mem, uint16_t value);

int mmio_mmwd(uintptr_t mem, uint32_t value);

#endif

