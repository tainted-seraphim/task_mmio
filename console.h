#ifndef TASK_MMIO_CONSOLE_H
#define TASK_MMIO_CONSOLE_H

#include <unistd.h>
#include "command.h"
#include "history.h"

enum {
	CONSOLE_MAX_INPUT_SIZE = 80
};

struct console {
	char input_buffer[CONSOLE_MAX_INPUT_SIZE];
	size_t current_position;
	size_t current_length;
	struct history *input_history;
	struct command *current_command;
};

void console_init(struct console *con);

void console_free(struct console *con);

void console_run(struct console *con);

void console_do_action_for_char(struct console *con, char ch);

void console_execute_command(struct console *con);

#endif

