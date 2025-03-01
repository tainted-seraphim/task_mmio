#ifndef TASK_MMIO_TERMINAL_H
#define TASK_MMIO_TERMINAL_H

#include <termios.h>

void terminal_init(void);

void terminal_enable_raw_terminal(struct termios *terminal);

void terminal_reset(void);

#endif

