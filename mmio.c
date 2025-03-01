#include <unistd.h>
#include <sys/io.h>
#include <stdint.h>
#include <inttypes.h>
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/io.h>
#include "mmio.h"

uint8_t mmio_iorb(uintptr_t port)
{
	int check = 0;
	uint16_t max_port = TASK_MMIO_PORT_END + 1 - sizeof(uint8_t);
	uint8_t result = -1;
	check = ioperm(TASK_MMIO_PORT_START, TASK_MMIO_PORT_COUNT, 1);
	if (check < 0) {
		perror("ioperm");
		printf("\r");
		return -1;
	}
	if (port > max_port) {
		printf("Port '0x%" PRIXPTR "' is out of range\r\n", port);
		printf("0x%X - 0x%X expected\r\n", TASK_MMIO_PORT_START, max_port);
		return -1;
	}
	result = inb(port);
	return result;
}

uint32_t mmio_iord(uintptr_t port)
{
	int check = 0;
	uint16_t max_port = TASK_MMIO_PORT_END + 1 - sizeof(uint32_t);
	uint32_t result = -1;
	check = ioperm(TASK_MMIO_PORT_START, TASK_MMIO_PORT_COUNT, 1);
	if (check < 0) {
		perror("ioperm");
		printf("\r");
		return -1;
	}
	if (port > max_port) {
		printf("Port '0x%" PRIXPTR "' is out of range\r\n", port);
		printf("0x%X - 0x%X expected\r\n", TASK_MMIO_PORT_START, max_port);
		return -1;
	}
	result = inl(port);
	return result;
}

uint16_t mmio_iorw(uintptr_t port)
{
	int check = 0;
	uint16_t max_port = TASK_MMIO_PORT_END + 1 - sizeof(uint16_t);
	uint16_t result = -1;
	check = ioperm(TASK_MMIO_PORT_START, TASK_MMIO_PORT_COUNT, 1);
	if (check < 0) {
		perror("ioperm");
		printf("\r");
		return -1;
	}
	if (port > max_port) {
		printf("Port '0x%" PRIXPTR "' is out of range\r\n", port);
		printf("0x%X - 0x%X expected\r\n", TASK_MMIO_PORT_START, max_port);
		return -1;
	}
	result = inw(port);
	return result;
}

void mmio_iowb(uintptr_t port, uint8_t value)
{
	int check = 0;
	uint16_t max_port = TASK_MMIO_PORT_END + 1 - sizeof(uint8_t);
	check = ioperm(TASK_MMIO_PORT_START, TASK_MMIO_PORT_COUNT, 1);
	if (check < 0) {
		perror("ioperm");
		printf("\r");
		return;
	}
	if (port > max_port) {
		printf("Port '0x%" PRIXPTR "' is out of range\r\n", port);
		printf("0x%X - 0x%X expected\r\n", TASK_MMIO_PORT_START, max_port);
		return;
	}
	outb(value, (uint16_t)port);
}

void mmio_iowd(uintptr_t port, uint32_t value)
{
	int check = 0;
	uint16_t max_port = TASK_MMIO_PORT_END + 1 - sizeof(uint32_t);
	check = ioperm(TASK_MMIO_PORT_START, TASK_MMIO_PORT_COUNT, 1);
	if (check < 0) {
		perror("ioperm");
		printf("\r");
		return;
	}
	if (port > max_port) {
		printf("Port '0x%" PRIXPTR "' is out of range\r\n", port);
		printf("0x%X - 0x%X expected\r\n", TASK_MMIO_PORT_START, max_port);
		return;
	}
	outl(value, (uint16_t)port);
}

void mmio_ioww(uintptr_t port, uint16_t value)
{
	int check = 0;
	unsigned short max_port = TASK_MMIO_PORT_END + 1 - sizeof(uint8_t);
	check = ioperm(TASK_MMIO_PORT_START, TASK_MMIO_PORT_COUNT, 1);
	if (check < 0) {
		perror("ioperm");
		printf("\r");
		return;
	}
	if (port > max_port) {
		printf("Port '0x%" PRIXPTR "' is out of range\r\n", port);
		printf("0x%X - 0x%X expected\r\n", TASK_MMIO_PORT_START, max_port);
		return;
	}
	outw(value, (uint16_t)port);
}

uint8_t mmio_mmrb(uintptr_t mem)
{
	void *map_base = NULL;
	size_t page_size = 0;
	size_t shift = 0;
	int mem_fd = -1;
	uint8_t result = -1;
	page_size = (size_t)sysconf(_SC_PAGESIZE);
	shift = mem % page_size;
	mem -= shift;
	mem_fd = open("/dev/mem", O_RDWR | O_SYNC);
	if (mem_fd < 0) {
		perror("open");
		printf("\r");
		return -1;
	}
	map_base = mmap(NULL, page_size, PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd, mem);
	if (map_base == (void *)(-1)) {
		perror("mmap");
		printf("\r");
		close(mem_fd);
		return -1;
	}
	result = *((uint8_t *)(map_base) + shift);
	munmap(map_base, page_size);
	close(mem_fd);
	return result;
}

uint32_t mmio_mmrd(uintptr_t mem)
{
	void *map_base = NULL;
	size_t page_size = 0;
	size_t shift = 0;
	int mem_fd = -1;
	uint32_t result = -1;
	page_size = (size_t)sysconf(_SC_PAGESIZE);
	shift = mem % page_size;
	mem -= shift;
	mem_fd = open("/dev/mem", O_RDWR | O_SYNC);
	if (mem_fd < 0) {
		perror("open");
		printf("\r");
		return -1;
	}
	map_base = mmap(NULL, page_size, PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd, mem);
	if (map_base == (void *)(-1)) {
		perror("mmap");
		printf("\r");
		close(mem_fd);
		return -1;
	}
	result = *((uint32_t *)(map_base) + shift);
	munmap(map_base, page_size);
	close(mem_fd);
	return result;
}

uint16_t mmio_mmrw(uintptr_t mem)
{
	void *map_base = NULL;
	size_t page_size = 0;
	size_t shift = 0;
	int mem_fd = -1;
	uint16_t result = -1;
	page_size = (size_t)sysconf(_SC_PAGESIZE);
	shift = mem % page_size;
	mem -= shift;
	mem_fd = open("/dev/mem", O_RDWR | O_SYNC);
	if (mem_fd < 0) {
		perror("open");
		printf("\r");
		return -1;
	}
	map_base = mmap(NULL, page_size, PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd, mem);
	if (map_base == (void *)(-1)) {
		perror("mmap");
		printf("\r");
		close(mem_fd);
		return -1;
	}
	result = *((uint16_t *)(map_base) + shift);
	munmap(map_base, page_size);
	close(mem_fd);
	return result;
}

void mmio_mmwb(uintptr_t mem, uint8_t value)
{
	void *map_base = NULL;
	size_t page_size = 0;
	size_t shift = 0;
	int mem_fd = -1;
	page_size = (size_t)sysconf(_SC_PAGESIZE);
	shift = mem % page_size;
	mem -= shift;
	mem_fd = open("/dev/mem", O_RDWR | O_SYNC);
	if (mem_fd < 0) {
		perror("open");
		printf("\r");
		return;
	}
	map_base = mmap(NULL, page_size, PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd, mem);
	if (map_base == (void *)(-1)) {
		perror("mmap");
		printf("\r");
		close(mem_fd);
		return;
	}
	*((uint8_t *)(map_base) + shift) = value;
	munmap(map_base, page_size);
	close(mem_fd);
}

void mmio_mmwd(uintptr_t mem, uint32_t value)
{
	void *map_base = NULL;
	size_t page_size = 0;
	size_t shift = 0;
	int mem_fd = -1;
	page_size = (size_t)sysconf(_SC_PAGESIZE);
	shift = mem % page_size;
	mem -= shift;
	mem_fd = open("/dev/mem", O_RDWR | O_SYNC);
	if (mem_fd < 0) {
		perror("open");
		printf("\r");
		return;
	}
	map_base = mmap(NULL, page_size, PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd, mem);
	if (map_base == (void *)(-1)) {
		perror("mmap");
		printf("\r");
		close(mem_fd);
		return;
	}
	*((uint32_t *)(map_base) + shift) = value;
	munmap(map_base, page_size);
	close(mem_fd);
}

void mmio_mmww(uintptr_t mem, uint16_t value)
{
	void *map_base = NULL;
	size_t page_size = 0;
	size_t shift = 0;
	int mem_fd = -1;
	page_size = (size_t)sysconf(_SC_PAGESIZE);
	shift = mem % page_size;
	mem -= shift;
	mem_fd = open("/dev/mem", O_RDWR | O_SYNC);
	if (mem_fd < 0) {
		perror("open");
		printf("\r");
		return;
	}
	map_base = mmap(NULL, page_size, PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd, mem);
	if (map_base == (void *)(-1)) {
		perror("mmap");
		printf("\r");
		close(mem_fd);
		return;
	}
	*((uint8_t *)(map_base) + shift) = value;
	munmap(map_base, page_size);
	close(mem_fd);
}

