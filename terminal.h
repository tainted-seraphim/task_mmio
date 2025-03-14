/* Date: 09.03.2025 */

#ifndef TASK_MMIO_TERMINAL_H
#define TASK_MMIO_TERMINAL_H

#include <termios.h>

void terminal_init(struct termios *terminal);

void terminal_free(struct termios *terminal);

void terminal_enable_raw_terminal(struct termios *terminal);

void terminal_disable_raw_terminal(struct termios *terminal);

void terminal_set_terminal(struct termios *terminal);

#endif

