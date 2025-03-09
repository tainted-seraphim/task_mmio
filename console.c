/* Date: 09.03.2025 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include <limits.h>
#include "console.h"
#include "input.h"
#include "command.h"
#include "history.h"
#include "chars.h"
#include "print.h"
#include "mmio.h"
#include "esc.h"
#include "terminal.h"

static const struct console_char_wrapper char_wrappers[] = {
	{CHAR_IDLE, console_char_idle},
	{CHAR_PRINTABLE, console_char_printable},
	{CHAR_NEWLINE, console_char_newline},
	{CHAR_BACKSPACE, console_char_backspace},
	{CHAR_ESC, console_char_esc},
	{CHAR_INTERRUPT, console_char_interrupt},
	{CHAR_QUIT, console_char_quit},
	{CHAR_UNSUPPORTED, console_char_unsupported}
};

static const struct console_esc_wrapper esc_wrappers[] = {
	{ESC_KEY, console_esc_esc_key},
	{ESC_ARROW_UP, console_esc_arrow_up},
	{ESC_ARROW_DOWN, console_esc_arrow_down},
	{ESC_ARROW_RIGHT, console_esc_arrow_right},
	{ESC_ARROW_LEFT, console_esc_arrow_left},
	{ESC_HOME, console_esc_home},
	{ESC_END, console_esc_end},
	{ESC_UNSUPPORTED, console_esc_unsupported},
};

static const struct console_command_wrapper command_wrappers[] = {
	{"help", console_command_help},
	{"quit", console_command_quit},
	{"clear", console_command_clear},
	{"history", console_command_history},
	{"iorb", console_command_iorb},
	{"iorw", console_command_iorw},
	{"iord", console_command_iord},
	{"iowb", console_command_iowb},
	{"ioww", console_command_ioww},
	{"iowd", console_command_iowd},
	{"mmrb", console_command_mmrb},
	{"mmrw", console_command_mmrw},
	{"mmrd", console_command_mmrd},
	{"mmwb", console_command_mmwb},
	{"mmww", console_command_mmww},
	{"mmwd", console_command_mmwd},
	{NULL, NULL}
};

void console_do_action_for_char(struct console *con, char ch)
{
	size_t index = 0;
	enum char_type type = 0;

	if (con == NULL) {
		return;
	}

	type = chars_get_char_type(ch);
	while (char_wrappers[index].type != CHAR_UNSUPPORTED) {
		if (char_wrappers[index].type == type) {
			char_wrappers[index].handler(con, ch);
			return;
		}
		index++;
	}
}

void console_do_action_for_esc(struct console *con, enum esc_sequence seq)
{
	size_t index = 0;

	if (con == NULL) {
		return;
	}

	while (esc_wrappers[index].seq != ESC_UNSUPPORTED) {
		if (esc_wrappers[index].seq == seq) {
			esc_wrappers[index].handler(con);
			return;
		}
		index++;
	}
}

void console_execute_command(struct console *con)
{
	size_t index = 0;
	if (con == NULL || con->current_command == NULL) {
		return;
	}
	while (command_wrappers[index].command != NULL) {
		if (0 == strcmp(command_wrappers[index].command, con->current_command->command)) {
			command_wrappers[index].handler(con, con->current_command->argument_count, con->current_command->arguments);
			return;
		}
		index++;
	}
	printf("Invalid command '%s'\r\n", con->current_command->command);

}

void console_free(struct console *con)
{
	if (con == NULL) {
		return;
	}

	input_free(con->input_buffer);
	free(con->input_buffer);
	con->input_buffer = NULL;

	input_free(con->input_backup);
	free(con->input_backup);
	con->input_backup = NULL;

	history_free(con->input_history);
	free(con->input_history);
	con->input_history = NULL;

	command_free(con->current_command);
	free(con->current_command);
	con->current_command = NULL;

	terminal_set_terminal(con->original_terminal);
	print_disable_alt_screen();

	terminal_free(con->original_terminal);
	free(con->original_terminal);
	con->original_terminal = NULL;

	terminal_free(con->raw_terminal);
	free(con->raw_terminal);
	con->raw_terminal = NULL;
}

void console_init(struct console *con)
{
	if (con == NULL) {
		return;
	}

	con->input_buffer = (struct input *)malloc(sizeof(struct input));
	if (con->input_buffer == NULL) {
		printf("console_init con->input_buffer malloc failed\r\n");
		exit(1);
	}
	input_init(con->input_buffer);

	con->input_backup = (struct input *)malloc(sizeof(struct input));
	if (con->input_backup == NULL) {
		printf("console_init con->input_backup malloc failed\r\n");
		exit(1);
	}
	input_init(con->input_backup);

	con->input_history = (struct history *)malloc(sizeof(struct history));
	if (con->input_history == NULL) {
		printf("console_init con->input_history malloc failed\r\n");
		exit(1);
	}
	history_init(con->input_history);

	con->current_command = (struct command *)
	                       malloc(sizeof(struct command));
	if (con->current_command == NULL) {
		printf("console_init con->current_command malloc failed\r\n");
		exit(1);
	}
	command_init(con->current_command);

	con->original_terminal = (struct termios *)malloc(sizeof(struct termios));
	if (con->original_terminal == NULL) {
		printf("console_init con->original_terminal malloc failed\r\n");
		exit(1);
	}
	terminal_init(con->original_terminal);

	con->raw_terminal = (struct termios *)malloc(sizeof(struct termios));
	if (con->raw_terminal == NULL) {
		printf("console_init con->raw_terminal malloc failed\r\n");
		exit(1);
	}
	terminal_init(con->raw_terminal);
	terminal_enable_raw_terminal(con->raw_terminal);
	terminal_set_terminal(con->raw_terminal);
	print_enable_alt_screen();
}

void console_run(struct console *con)
{
	size_t bytes_read = 0;
	char ch = 0;
	if (con == NULL) {
		return;
	}
	print_header();
	print_input_prompt();
	while (1) {
		bytes_read = read(0, &ch, 1);
		if (bytes_read == 0) {
			ch = 0;
		}
		console_do_action_for_char(con, ch);
		print_line_reset();
		print_input_prompt();
		printf("%s", con->input_buffer);
		print_set_cursor_position(con->input_buffer->cursor_position + 1);
		fflush(stdout);
	}
}

int console_command_help(struct console *con, size_t argc, char **argv)
{
	size_t index = 0;
	if (con == NULL) {
		return -1;
	}
	if (argc != 0) {
		printf("Command 'help' takes 0 arguments\r\n");
		printf("Unwanted arguments:");
		for (index = 0; index < argc; index++) {
			printf(" '%s'", argv[index]);
		}
		printf("\r\n");
		return -1;
	}
	print_help();
	return 0;
}

int console_command_quit(struct console *con, size_t argc, char **argv)
{
	size_t index = 0;
	if (con == NULL) {
		return -1;
	}
	if (argc != 0) {
		printf("Command 'quit' takes 0 arguments\r\n");
		printf("Unwanted arguments:");
		for (index = 0; index < argc; index++) {
			printf(" '%s'", argv[index]);
		}
		printf("\r\n");
		return -1;
	}
	console_free(con);
	exit(0);
	return 0;
}

int console_command_clear(struct console *con, size_t argc, char **argv)
{
	size_t index = 0;
	if (con == NULL) {
		return -1;
	}
	if (argc != 0) {
		printf("Command 'clear' takes 0 arguments\r\n");
		printf("Unwanted arguments:");
		for (index = 0; index < argc; index++) {
			printf(" '%s'", argv[index]);
		}
		printf("\r\n");
		return -1;
	}
	print_screen_clear();
	return 0;
}

int console_command_history(struct console *con, size_t argc, char **argv)
{
	size_t index = 0;
	if (con == NULL) {
		return -1;
	}
	if (argc != 0) {
		printf("Command 'history' takes 0 arguments\r\n");
		printf("Unwanted arguments:");
		for (index = 0; index < argc; index++) {
			printf(" '%s'", argv[index]);
		}
		printf("\r\n");
		return -1;
	}
	history_print(con->input_history);
	return 0;

}

int console_command_iorb(struct console *con, size_t argc, char **argv)
{
	uintptr_t port = 0;
	int check = 0;
	uint8_t result = 0;
	if (con == NULL) {
		return -1;
	}
	if (argc != 1) {
		printf("Command 'iorb' takes 1 argument\r\n");
		return -1;
	}
	port = strtoull(argv[0], NULL, 0);
	if (port == ULLONG_MAX || *(argv[0]) == '-') {
		printf("Invalid port '%s'\r\n", argv[0]);
		return -1;
	}
	check = mmio_iorb(port, &result);
	if (check != 0) {
		printf("iorb error occured\r\n");
		return -1;
	} else {
		printf("0x%02X\r\n", result);
	}
	return 0;
}

int console_command_iorw(struct console *con, size_t argc, char **argv)
{
	uintptr_t port = 0;
	int check = 0;
	uint16_t result = 0;
	if (con == NULL) {
		return -1;
	}
	if (argc != 1) {
		printf("Command 'iorw' takes 1 argument\r\n");
		return -1;
	}
	port = strtoull(argv[0], NULL, 0);
	if (port == ULLONG_MAX || *(argv[0]) == '-') {
		printf("Invalid port '%s'\r\n", argv[0]);
		return -1;
	}
	check = mmio_iorw(port, &result);
	if (check != 0) {
		printf("iorw error occured\r\n");
		return -1;
	} else {
		printf("0x%04X\r\n", result);
	}
	return 0;
}

int console_command_iord(struct console *con, size_t argc, char **argv)
{
	uintptr_t port = 0;
	int check = 0;
	uint32_t result = 0;
	if (con == NULL) {
		return -1;
	}
	if (argc != 1) {
		printf("Command 'iord' takes 1 argument\r\n");
		return -1;
	}
	port = strtoull(argv[0], NULL, 0);
	if (port == ULLONG_MAX || *(argv[0]) == '-') {
		printf("Invalid port '%s'\r\n", argv[0]);
		return -1;
	}
	check = mmio_iord(port, &result);
	if (check != 0) {
		printf("iord error occured\r\n");
		return -1;
	} else {
		printf("0x%08X\r\n", result);
	}
	return 0;
}

int console_command_iowb(struct console *con, size_t argc, char **argv)
{
	uintptr_t port = 0;
	uintmax_t value = 0;
	int check = 0;
	if (con == NULL) {
		return -1;
	}
	if (argc != 2) {
		printf("Command 'iowb' takes 2 arguments\r\n");
		return -1;
	}
	port = strtoull(argv[0], NULL, 0);
	if (port == ULLONG_MAX || *(argv[0]) == '-') {
		printf("Invalid port '%s'\r\n", argv[0]);
		return -1;
	}
	value = strtoull(argv[1], NULL, 0);
	if (value == ULLONG_MAX || *(argv[1]) == '-') {
		printf("Invalid value '%s'\r\n", argv[1]);
		return -1;
	}
	if (value > UINT8_MAX) {
		printf("Value '0x%" PRIXMAX "' is greater than maximum allowed %zu\r\n", value, (size_t)UINT8_MAX);
		return -1;
	}
	check = mmio_iowb(port, (uint8_t)value);
	if (check != 0) {
		printf("iowb error occured\r\n");
		return -1;
	}
	return 0;
}

int console_command_ioww(struct console *con, size_t argc, char **argv)
{
	uintptr_t port = 0;
	uintmax_t value = 0;
	int check = 0;
	if (con == NULL) {
		return -1;
	}
	if (argc != 2) {
		printf("Command 'ioww' takes 2 arguments\r\n");
		return -1;
	}
	port = strtoull(argv[0], NULL, 0);
	if (port == ULLONG_MAX || *(argv[0]) == '-') {
		printf("Invalid port '%s'\r\n", argv[0]);
		return -1;
	}
	value = strtoull(argv[1], NULL, 0);
	if (value == ULLONG_MAX || *(argv[1]) == '-') {
		printf("Invalid value '%s'\r\n", argv[1]);
		return -1;
	}
	if (value > UINT16_MAX) {
		printf("Value '0x%" PRIXMAX "' is greater than maximum allowed %zu\r\n", value, (size_t)UINT16_MAX);
		return -1;
	}
	check = mmio_ioww(port, (uint16_t)value);
	if (check != 0) {
		printf("ioww error occured\r\n");
		return -1;
	}
	return 0;
}

int console_command_iowd(struct console *con, size_t argc, char **argv)
{
	uintptr_t port = 0;
	uintmax_t value = 0;
	int check = 0;
	if (con == NULL) {
		return -1;
	}
	if (argc != 2) {
		printf("Command 'iowd' takes 2 arguments\r\n");
		return -1;
	}
	port = strtoull(argv[0], NULL, 0);
	if (port == ULLONG_MAX || *(argv[0]) == '-') {
		printf("Invalid port '%s'\r\n", argv[0]);
		return -1;
	}
	value = strtoull(argv[1], NULL, 0);
	if (value == ULLONG_MAX || *(argv[1]) == '-') {
		printf("Invalid value '%s'\r\n", argv[1]);
		return -1;
	}
	if (value > UINT32_MAX) {
		printf("Value '0x%" PRIXMAX "' is greater than maximum allowed %zu\r\n", value, (size_t)UINT32_MAX);
		return -1;
	}
	check = mmio_iowd(port, (uint32_t)value);
	if (check != 0) {
		printf("iowd error occured\r\n");
		return -1;
	}
	return 0;
}

int console_command_mmrb(struct console *con, size_t argc, char **argv)
{
	uintptr_t mem = 0;
	int check = 0;
	uint8_t result = 0;
	if (con == NULL) {
		return -1;
	}
	if (argc != 1) {
		printf("Command 'mmrb' takes 1 argument\r\n");
		return -1;
	}
	mem = strtoull(argv[0], NULL, 0);
	if (mem == ULLONG_MAX || *(argv[0]) == '-') {
		printf("Invalid memory address '%s'\r\n", argv[0]);
		return -1;
	}
	check = mmio_mmrb(mem, &result);
	if (check != 0) {
		printf("mmrb error occured\r\n");
		return -1;
	} else {
		printf("0x%02X\r\n", result);
	}
	return 0;
}

int console_command_mmrw(struct console *con, size_t argc, char **argv)
{
	uintptr_t mem = 0;
	int check = 0;
	uint16_t result = 0;
	if (con == NULL) {
		return -1;
	}
	if (argc != 1) {
		printf("Command 'mmrw' takes 1 argument\r\n");
		return -1;
	}
	mem = strtoull(argv[0], NULL, 0);
	if (mem == ULLONG_MAX || *(argv[0]) == '-') {
		printf("Invalid memory address '%s'\r\n", argv[0]);
		return -1;
	}
	check = mmio_mmrw(mem, &result);
	if (check != 0) {
		printf("mmrw error occured\r\n");
		return -1;
	} else {
		printf("0x%04X\r\n", result);
	}
	return 0;
}

int console_command_mmrd(struct console *con, size_t argc, char **argv)
{
	uintptr_t mem = 0;
	int check = 0;
	uint32_t result = 0;
	if (con == NULL) {
		return -1;
	}
	if (argc != 1) {
		printf("Command 'mmrd' takes 1 argument\r\n");
		return -1;
	}
	mem = strtoull(argv[0], NULL, 0);
	if (mem == ULLONG_MAX || *(argv[0]) == '-') {
		printf("Invalid memory address '%s'\r\n", argv[0]);
		return -1;
	}
	check = mmio_mmrd(mem, &result);
	if (check != 0) {
		printf("mmrd error occured\r\n");
		return -1;
	} else {
		printf("0x%08X\r\n", result);
	}
	return 0;
}

int console_command_mmwb(struct console *con, size_t argc, char **argv)
{
	uintptr_t mem = 0;
	uintmax_t value = 0;
	int check = 0;
	if (con == NULL) {
		return -1;
	}
	if (argc != 2) {
		printf("Command 'mmwb' takes 2 arguments\r\n");
		return -1;
	}
	mem = strtoull(argv[0], NULL, 0);
	if (mem == ULLONG_MAX || *(argv[0]) == '-') {
		printf("Invalid memory address '%s'\r\n", argv[0]);
		return -1;
	}
	value = strtoull(argv[1], NULL, 0);
	if (value > UINT8_MAX) {
		printf("Value '0x%" PRIXMAX "' is greater than maximum allowed %zu\r\n", value, (size_t)UINT8_MAX);
		return -1;
	}
	check = mmio_mmwb(mem, (uint8_t)value);
	if (check != 0) {
		printf("mmwb error occured\r\n");
		return -1;
	}
	return 0;
}

int console_command_mmww(struct console *con, size_t argc, char **argv)
{
	uintptr_t mem = 0;
	uintmax_t value = 0;
	int check = 0;
	if (con == NULL) {
		return -1;
	}
	if (argc != 2) {
		printf("Command 'mmww' takes 2 arguments\r\n");
		return -1;
	}
	mem = strtoull(argv[0], NULL, 0);
	if (mem == ULLONG_MAX || *(argv[0]) == '-') {
		printf("Invalid memory address '%s'\r\n", argv[0]);
		return -1;
	}
	value = strtoull(argv[1], NULL, 0);
	if (value > UINT16_MAX) {
		printf("Value '0x%" PRIXMAX "' is greater than maximum allowed %zu\r\n", value, (size_t)UINT16_MAX);
		return -1;
	}
	check = mmio_mmww(mem, (uint16_t)value);
	if (check != 0) {
		printf("mmww error occured\r\n");
		return -1;
	}
	return 0;
}

int console_command_mmwd(struct console *con, size_t argc, char **argv)
{
	uintptr_t mem = 0;
	uintmax_t value = 0;
	int check = 0;
	if (con == NULL) {
		return -1;
	}
	if (argc != 2) {
		printf("Command 'mmwd' takes 2 arguments\r\n");
		return -1;
	}
	mem = strtoull(argv[0], NULL, 0);
	if (mem == ULLONG_MAX || *(argv[0]) == '-') {
		printf("Invalid memory address '%s'\r\n", argv[0]);
		return -1;
	}
	value = strtoull(argv[1], NULL, 0);
	if (value > UINT32_MAX) {
		printf("Value '0x%" PRIXMAX "' is greater than maximum allowed %zu\r\n", value, (size_t)UINT32_MAX);
		return -1;
	}
	check = mmio_mmwd(mem, (uint32_t)value);
	if (check != 0) {
		printf("mmwd error occured\r\n");
		return -1;
	}
	return 0;
}

int console_char_idle(struct console *con, char ch)
{
	return 0;
}

int console_char_printable(struct console *con, char ch)
{
	if (con == NULL) {
		return -1;
	}

	input_insert_byte(con->input_buffer, ch);
	return 0;
}

int console_char_newline(struct console *con, char ch)
{
	if (con == NULL) {
		return -1;
	}

	input_clear(con->input_backup);
	command_string_to_command(con->input_buffer->buffer, con->current_command);
	print_newline();
	if (con->current_command->command != NULL) {
		history_add(con->input_history, con->input_buffer->buffer);
		console_execute_command(con);
	}
	input_clear(con->input_buffer);
	command_free(con->current_command);
	return 0;
}

int console_char_backspace(struct console *con, char ch)
{
	if (con == NULL) {
		return -1;
	}

	input_backspace(con->input_buffer);
	return 0;
}

int console_char_esc(struct console *con, char ch)
{
	enum esc_sequence seq = 0;

	if (con == NULL) {
		return -1;
	}

	seq = esc_get_esc_sequence();
	console_do_action_for_esc(con, seq);
	return 0;
}

int console_char_interrupt(struct console *con, char ch)
{
	if (con == NULL) {
		return -1;
	}

	input_clear(con->input_buffer);
	printf("\r\nInput interrupted\r\n");
	return 0;
}

int console_char_quit(struct console *con, char ch)
{
	if (con == NULL) {
		return -1;
	}

	console_free(con);
	exit(0);
	return 0;
}

int console_char_unsupported(struct console *con, char ch)
{
	return 0;
}

int console_esc_esc_key(struct console *con)
{
	if (con == NULL) {
		return -1;
	}

	return 0;
}

int console_esc_arrow_up(struct console *con)
{
	if (con == NULL) {
		return -1;
	}

	if (con->input_history->current_node == con->input_history->tail_node) {
		return 0;
	}
	if (con->input_history->current_node == con->input_history->head_node) {
		input_copy_input(con->input_backup, con->input_buffer);
	}
	if (con->input_history->current_node->prev_node != con->input_history->tail_node) {
		history_current_node_up(con->input_history);
		input_set_buffer(con->input_buffer, con->input_history->current_node->input);
	}
	return 0;
}

int console_esc_arrow_down(struct console *con)
{
	if (con == NULL) {
		return -1;
	}

	if (con->input_history->current_node == con->input_history->head_node) {
		return 0;
	}
	if (con->input_history->current_node->next_node == con->input_history->head_node) {
		history_current_node_down(con->input_history);
		input_copy_input(con->input_buffer, con->input_backup);
	}
	if (con->input_history->current_node != con->input_history->head_node) {
		history_current_node_down(con->input_history);
		input_set_buffer(con->input_buffer, con->input_history->current_node->input);
	}
	return 0;
}

int console_esc_arrow_right(struct console *con)
{
	if (con == NULL) {
		return -1;
	}

	input_move_cursor_right(con->input_buffer);
	print_set_cursor_position(con->input_buffer->cursor_position);
	return 0;
}

int console_esc_arrow_left(struct console *con)
{
	if (con == NULL) {
		return -1;
	}

	input_move_cursor_left(con->input_buffer);
	print_set_cursor_position(con->input_buffer->cursor_position);
	return 0;
}

int console_esc_home(struct console *con)
{
	if (con == NULL) {
		return -1;
	}

	input_move_cursor_home(con->input_buffer);
	print_set_cursor_position(con->input_buffer->cursor_position);
	return 0;
}

int console_esc_end(struct console *con)
{
	if (con == NULL) {
		return -1;
	}

	input_move_cursor_end(con->input_buffer);
	print_set_cursor_position(con->input_buffer->cursor_position);
	return 0;
}

int console_esc_unsupported(struct console *con)
{
	if (con == NULL) {
		return -1;
	}

	printf("\r\nThis escape sequence is not supported yet\r\n");
	return 0;
}

