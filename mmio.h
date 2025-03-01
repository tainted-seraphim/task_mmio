#ifndef TASK_MMIO_MMIO_H
#define TASK_MMIO_MMIO_H

#include <stddef.h>
#include <stdint.h>

#define TASK_MMIO_PORT_START 0
#define TASK_MMIO_PORT_END   0xFFFF
#define TASK_MMIO_PORT_COUNT (TASK_MMIO_PORT_END - TASK_MMIO_PORT_START + 1)

uint8_t mmio_iorb(uintptr_t port);

uint16_t mmio_iorw(uintptr_t port);

uint32_t mmio_iord(uintptr_t port);

void mmio_iowb(uintptr_t port, uint8_t value);

void mmio_ioww(uintptr_t port, uint16_t value);

void mmio_iowd(uintptr_t port, uint32_t value);

uint8_t mmio_mmrb(uintptr_t mem);

uint16_t mmio_mmrw(uintptr_t mem);

uint32_t mmio_mmrd(uintptr_t mem);

void mmio_mmwb(uintptr_t mem, uint8_t value);

void mmio_mmww(uintptr_t mem, uint16_t value);

void mmio_mmwd(uintptr_t mem, uint32_t value);

#endif

