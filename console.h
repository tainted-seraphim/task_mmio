#ifndef TASK_MMIO_CONSOLE_H
#define TASK_MMIO_CONSOLE_H

#include <unistd.h>
#include "command.h"
#include "history.h"
#include "chars.h"
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

typedef int (*console_command_handler)(struct console *, size_t, char **);

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

typedef int (*console_char_handler)(struct console *, char); 

struct console_char_wrapper {
	enum char_type type;
	console_char_handler haldler;
};

int console_char_idle(struct console *con, char ch);

int console_char_printable(struct console *con, char ch);

int console_char_newline(struct console *con, char ch);

int console_char_backspace(struct console *con, char ch);

int console_char_esc(struct console *con, char ch);

int console_char_interrupt(struct console *con, char ch);

int console_char_quit(struct console *con, char ch);

int console_char_unsupported(struct console *con, char ch);

typedef int (*console_esc_handler)(struct console *, enum esc_sequence);

struct console_esc_wrapper {
	enum esc_sequence seq;
	console_esc_handler handler;
};

int console_esc_esc_key(struct console *con, enum esc_sequence seq);

int console_esc_arrow_up(struct console *con, enum esc_sequence seq);

int console_esc_arrow_down(struct console *con, enum esc_sequence seq);

int console_esc_arrow_right(struct console *con, enum esc_sequence seq);

int console_esc_arrow_left(struct console *con, enum esc_sequence seq);

int console_esc_home(struct console *con, enum esc_sequence seq);

int console_esc_end(struct console *con, enum esc_sequence seq);

int console_esc_unsupported(struct console *con, enum esc_sequence seq);

#endif

