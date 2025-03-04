#ifndef TASK_MMIO_CONSOLE_H
#define TASK_MMIO_CONSOLE_H

#include <unistd.h>
#include "command.h"
#include "history.h"
#include "esc.h"

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

void console_do_action_for_esc(struct console *con, enum esc_sequence seq);

void console_execute_command(struct console *con);

typedef int (*console_command_handler)(struct console *con, size_t, char **);

struct console_command_wrapper {
	const char *command;
	console_command_handler handler;
};

int console_command_help(struct console *con, size_t argc, char **argv);

int console_command_quit(struct console *con, size_t argc, char **argv);

int console_command_clear(struct console *con, size_t argc, char **argv);

int console_command_history(struct console *con, size_t argc, char **argv);

int console_command_iorb(struct console *con, size_t argc, char **argv);

int console_command_iorw(struct console *con, size_t argc, char **argv);

int console_command_iord(struct console *con, size_t argc, char **argv);

int console_command_iowb(struct console *con, size_t argc, char **argv);

int console_command_ioww(struct console *con, size_t argc, char **argv);

int console_command_iowd(struct console *con, size_t argc, char **argv);

int console_command_mmrb(struct console *con, size_t argc, char **argv);

int console_command_mmrw(struct console *con, size_t argc, char **argv);

int console_command_mmrd(struct console *con, size_t argc, char **argv);

int console_command_mmwb(struct console *con, size_t argc, char **argv);

int console_command_mmww(struct console *con, size_t argc, char **argv);

int console_command_mmwd(struct console *con, size_t argc, char **argv);

#endif

