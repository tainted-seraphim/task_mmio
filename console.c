#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include <limits.h>
#include "console.h"
#include "command.h"
#include "history.h"
#include "chars.h"
#include "print.h"
#include "mmio.h"

static struct console_command_wrapper wrappers[] = {
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
	{NULL, NULL};
}

void console_do_action_for_char(struct console *con, char ch)
{
	static struct history_node *current_node = NULL;
	static char input_buffer[CONSOLE_MAX_INPUT_SIZE] = {0};
	size_t index = 0;
	enum char_type type = 0;
	if (con == NULL) {
		return;
	}
	/* TODO: fix this mess 
	type = chars_get_char_type(ch);
	switch (type) {
	case CHAR_IDLE:
		break;
	case CHAR_PRINTABLE:
		if (con->current_position == CONSOLE_MAX_INPUT_SIZE - 1) {
			break;
		}
		for (index = CONSOLE_MAX_INPUT_SIZE - 1;
		                index > con->current_position; index--) {
			con->input_buffer[index] =
			        con->input_buffer[index - 1];
		}
		con->input_buffer[con->current_position] = ch;
		memmove(input_buffer, con->input_buffer, CONSOLE_MAX_INPUT_SIZE);
		con->current_position++;
		con->current_length++;
		break;
	case CHAR_NEWLINE:
		con->input_buffer[CONSOLE_MAX_INPUT_SIZE - 1] = 0;
		command_string_to_command(con->input_buffer,
		                          con->current_command);
		print_newline();
		if (con->current_command->command != NULL) {
			history_add(con->input_history, con->input_buffer);
			current_node = con->input_history->head_node;
			console_execute_command(con);
		}
		memset(con->input_buffer, 0, CONSOLE_MAX_INPUT_SIZE);
		memmove(input_buffer, con->input_buffer, strlen(con->input_buffer) + 1);
		con->current_length = 0;
		con->current_position = 0;
		print_input_prompt();
		break;
	case CHAR_BACKSPACE:
		if (con->current_position == 0) {
			break;
		}
		for (index = con->current_position - 1;
		                index < CONSOLE_MAX_INPUT_SIZE - 1; index++) {
			con->input_buffer[index] =
			        con->input_buffer[index + 1];
		}
		con->input_buffer[CONSOLE_MAX_INPUT_SIZE - 1] = 0;
		con->current_position--;
		con->current_length--;
		break;
	case CHAR_ARROW_UP:
	if (con->input_history->node_count == 0) {
		break;
	}
	if (current_node->prev_node == con->input_history->tail_node) {
		break;
	}
	current_node = current_node->prev_node;
	memmove(con->input_buffer, current_node->input,
	        strlen(current_node->input) + 1);
	con->current_length = strlen(con->input_buffer);
	con->current_position = con->current_length;
	break;
	case CHAR_ARROW_DOWN:
	if (con->input_history->node_count == 0) {
		break;
	}
	if (current_node == con->input_history->head_node) {
		break;
	}
	if (current_node->next_node == con->input_history->head_node) {
		memmove(con->input_buffer, input_buffer,
		        strlen(input_buffer) + 1);
		current_node = current_node->next_node;
	} else {
		current_node = current_node->next_node;
		memmove(con->input_buffer, current_node->input,
		        strlen(current_node->input) + 1);
	}
	con->current_length = strlen(con->input_buffer);
	con->current_position = con->current_length;
	break;
	case CHAR_ARROW_RIGHT:
	if (con->current_position == con->current_length) {
		break;
	}
	print_move_cursor_right();
	con->current_position++;
	break;
	case CHAR_ARROW_LEFT:
	if (con->current_position == 0) {
		break;
	}
	print_move_cursor_left();
	con->current_position--;
	break;
	case CHAR_INTERRUPT:
		printf("\r\nInput interrupted\r\n");
		print_line_reset();
		print_input_prompt();
		memset(con->input_buffer, 0, CONSOLE_MAX_INPUT_SIZE);
		memset(input_buffer, 0, CONSOLE_MAX_INPUT_SIZE);
		con->current_position = 0;
		con->current_length = 0;
		break;
	case CHAR_QUIT:
		exit(0);
		break;
	case CHAR_UNSUPPORTED:
		break;
	};
	*/
}

void console_execute_command(struct console *con)
{
	uintptr_t mem = 0;
	uintptr_t port = 0;
	uintmax_t value = 0;
	char *end = NULL;
	if (con == NULL || con->current_command == NULL) {
		return;
	}
	/* TODO: fix this mess
	if (0 == strcmp(con->current_command->command, "quit")) {
		if (con->current_command->argument_count != 0) {
			printf("Command 'quit' does not need arguments\r\n");
			return;
		}
		exit(0);
	}
	if (0 == strcmp(con->current_command->command, "help")) {
		if (con->current_command->argument_count != 0) {
			printf("Command 'help' does not need arguments\r\n");
			return;
		}
		print_help();
		return;
	}
	if (0 == strcmp(con->current_command->command, "clear")) {
		if (con->current_command->argument_count != 0) {
			printf("Command 'clear' does not need arguments\r\n");
			return;
		}
		print_screen_clear();
		return;
	}
	if (0 == strcmp(con->current_command->command, "history")) {
		if (con->current_command->argument_count != 0) {
			printf(
			        "Command 'history' does not need arguments\r\n"
			);
			return;
		}
		history_print(con->input_history);
		return;
	}
	if (0 == strcmp(con->current_command->command, "iorb")) {
		if (con->current_command->argument_count != 1) {
			printf("Command 'iorb' takes 1 argument\r\n");
			return;
		}
		port = (uintptr_t)strtoull(con->current_command->arguments[0], &end, 0);
		if (port == ULLONG_MAX) {
			printf("Invalid port '%s'\r\n",
			       con->current_command->arguments[0]);
		}
		printf("0x%" PRIX8 "\r\n", mmio_iorb(port));
		return;
	}
	if (0 == strcmp(con->current_command->command, "iorw")) {
		if (con->current_command->argument_count != 1) {
			printf("Command 'iorw' takes 1 argument\r\n");
			return;
		}
		port = (uintptr_t)strtoull(con->current_command->arguments[0], &end, 0);
		if (port == ULLONG_MAX) {
			printf("Invalid port '%s'\r\n",
			       con->current_command->arguments[0]);
		}
		printf("0x%" PRIX16 "\r\n", mmio_iorw(port));
		return;
	}
	if (0 == strcmp(con->current_command->command, "iord")) {
		if (con->current_command->argument_count != 1) {
			printf("Command 'iord' takes 1 argument\r\n");
			return;
		}
		port = (uintptr_t)strtoull(con->current_command->arguments[0], &end, 0);
		if (port == ULLONG_MAX) {
			printf("Invalid port '%s'\r\n",
			       con->current_command->arguments[0]);
		}
		printf("0x%" PRIX32 "\r\n", mmio_iord(port));
		return;
	}
	if (0 == strcmp(con->current_command->command, "iowb")) {
		if (con->current_command->argument_count != 2) {
			printf("Command 'iowb' takes 2 arguments\r\n");
			return;
		}
		port = (uintptr_t)strtoull(con->current_command->arguments[0], &end, 0);
		if (port == ULLONG_MAX) {
			printf("Invalid port '%s'\r\n",
			       con->current_command->arguments[0]);
		}
		end = NULL;
		value = (uintmax_t)strtoull(con->current_command->arguments[1], &end, 0);
		if (value == ULLONG_MAX) {
			printf("Invalid value '%s'\r\n",
			       con->current_command->arguments[1]);
		}
		mmio_iowb(port, value);
		return;
	}
	if (0 == strcmp(con->current_command->command, "ioww")) {
		if (con->current_command->argument_count != 2) {
			printf("Command 'ioww' takes 2 arguments\r\n");
			return;
		}
		port = (uintptr_t)strtoull(con->current_command->arguments[0], &end, 0);
		if (port == ULLONG_MAX) {
			printf("Invalid port '%s'\r\n",
			       con->current_command->arguments[0]);
		}
		end = NULL;
		value = (uintmax_t)strtoull(con->current_command->arguments[1], &end, 0);
		if (value == ULLONG_MAX) {
			printf("Invalid value '%s'\r\n",
			       con->current_command->arguments[1]);
		}
		mmio_ioww(port, value);
		return;
	}
	if (0 == strcmp(con->current_command->command, "iowd")) {
		if (con->current_command->argument_count != 2) {
			printf("Command 'iowd' takes 2 arguments\r\n");
			return;
		}
		port = (uintptr_t)strtoull(con->current_command->arguments[0], &end, 0);
		if (port == ULLONG_MAX) {
			printf("Invalid port '%s'\r\n",
			       con->current_command->arguments[0]);
		}
		end = NULL;
		value = (uintmax_t)strtoull(con->current_command->arguments[1], &end, 0);
		if (value == ULLONG_MAX) {
			printf("Invalid value '%s'\r\n",
			       con->current_command->arguments[1]);
		}
		mmio_iowd(port, value);
		return;
	}
	if (0 == strcmp(con->current_command->command, "mmrb")) {
		if (con->current_command->argument_count != 1) {
			printf("Command 'mmrb' takes 1 argument\r\n");
			return;
		}
		mem = (uintptr_t)strtoull(con->current_command->arguments[0], &end, 0);
		if (mem == ULLONG_MAX) {
			printf("Invalid memory address '%s'\r\n",
			       con->current_command->arguments[0]);
		}
		printf("0x%" PRIX8 "\r\n", mmio_mmrb(mem));
		return;
	}
	if (0 == strcmp(con->current_command->command, "mmrw")) {
		if (con->current_command->argument_count != 1) {
			printf("Command 'mmrw' takes 1 argument\r\n");
			return;
		}
		mem = (uintptr_t)strtoull(con->current_command->arguments[0], &end, 0);
		if (mem == ULLONG_MAX) {
			printf("Invalid memory address '%s'\r\n",
			       con->current_command->arguments[0]);
		}
		printf("0x%" PRIX16 "\r\n", mmio_mmrw(mem));
		return;
	}
	if (0 == strcmp(con->current_command->command, "mmrd")) {
		if (con->current_command->argument_count != 1) {
			printf("Command 'mmrd' takes 1 argument\r\n");
			return;
		}
		mem = (uintptr_t)strtoull(con->current_command->arguments[0], &end, 0);
		if (mem == ULLONG_MAX) {
			printf("Invalid memory address '%s'\r\n",
			       con->current_command->arguments[0]);
		}
		printf("0x%" PRIX32 "\r\n", mmio_mmrd(mem));
		return;
	}
	if (0 == strcmp(con->current_command->command, "mmwb")) {
		if (con->current_command->argument_count != 2) {
			printf("Command 'mmwb' takes 2 arguments\r\n");
			return;
		}
		mem = (uintptr_t)strtoull(con->current_command->arguments[0], &end, 0);
		if (mem == ULLONG_MAX) {
			printf("Invalid memory address '%s'\r\n",
			       con->current_command->arguments[0]);
		}
		end = NULL;
		value = (uintmax_t)strtoull(con->current_command->arguments[1], &end, 0);
		if (value == ULLONG_MAX) {
			printf("Invalid value '%s'\r\n",
			       con->current_command->arguments[1]);
		}
		mmio_mmwb(mem, value);
		return;
	}
	if (0 == strcmp(con->current_command->command, "mmww")) {
		if (con->current_command->argument_count != 2) {
			printf("Command 'mmww' takes 2 arguments\r\n");
			return;
		}
		mem = (uintptr_t)strtoull(con->current_command->arguments[0], &end, 0);
		if (mem == ULLONG_MAX) {
			printf("Invalid memory address '%s'\r\n",
			       con->current_command->arguments[0]);
		}
		end = NULL;
		value = (uintmax_t)strtoull(con->current_command->arguments[1], &end, 0);
		if (value == ULLONG_MAX) {
			printf("Invalid value '%s'\r\n",
			       con->current_command->arguments[1]);
		}
		mmio_mmww(mem, value);
		return;
	}
	if (0 == strcmp(con->current_command->command, "mmwd")) {
		if (con->current_command->argument_count != 2) {
			printf("Command 'mmwd' takes 2 arguments\r\n");
			return;
		}
		mem = (uintptr_t)strtoull(con->current_command->arguments[0], &end, 0);
		if (mem == ULLONG_MAX) {
			printf("Invalid memory address '%s'\r\n",
			       con->current_command->arguments[0]);
		}
		end = NULL;
		value = (uintmax_t)strtoull(con->current_command->arguments[1], &end, 0);
		if (value == ULLONG_MAX) {
			printf("Invalid value '%s'\r\n",
			       con->current_command->arguments[1]);
		}
		mmio_mmwd(mem, value);
		return;
	}
	*/
	printf("Invalid command '%s'\r\n", con->current_command->command);

}

void console_free(struct console *con)
{
	if (con == NULL) {
		return;
	}
	memset(con->input_buffer, 0, CONSOLE_MAX_INPUT_SIZE);
	con->current_position = 0;
	con->current_length = 0;
	history_free(con->input_history);
	free(con->input_history);
	con->input_history = NULL;
	command_free(con->current_command);
	free(con->current_command);
	con->current_command = NULL;
}

void console_init(struct console *con)
{
	if (con == NULL) {
		return;
	}
	memset(con->input_buffer, 0, CONSOLE_MAX_INPUT_SIZE);
	con->current_position = 0;
	con->current_length = 0;
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
}

void console_run(struct console *con)
{
	int bytes_read = 0;
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
		print_set_cursor_position(strlen(TASK_MMIO_INPUT_PROMPT) + con->current_position + 1);
		fflush(stdout);
	}
}

int console_command_help(struct console *con, size_t argc, char **argv)
{
	if (con == NULL) {
		return -1;
	}
	if (argc != 0) {
		printf("Command 'help' takes 0 arguments\r\n");
		return -1;
	}
	print_help();
	return 0;
}

int console_command_quit(struct console *con, size_t argc, char **argv)
{
	if (con == NULL) {
		return -1;
	}
	if (argc != 0) {
		printf("Command 'quit' takes 0 arguments\r\n");
		return -1;
	}
	exit(0);
	return 0;
}

int console_command_clear(struct console *con, size_t argc, char **argv)
{
	if (con == NULL) {
		return -1;
	}
	if (argc != 0) {
		printf("Command 'clear' takes 0 arguments\r\n");
		return -1;
	}
	print_screen_clear();
	return 0;
}

int console_command_history(struct console *con, size_t argc, char **argv)
{
	if (con == NULL) {
		return -1;
	}
	if (argc != 0) {
		printf("Command 'history' takes 0 arguments\r\n");
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
		printf("0x%" PRIX8 "\r\n", result);
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
		printf("0x%" PRIX16 "\r\n", result);
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
		printf("0x%" PRIX32 "\r\n", result);
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
	if (argc != 1) {
		printf("Command 'iowb' takes 1 argument\r\n");
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
		printf("Value '0x%" PRIXMAX "' is greater than maximum allowed %" PRIXMAX "\r\n", value, UINT8_MAX);
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
	if (argc != 1) {
		printf("Command 'ioww' takes 1 argument\r\n");
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
		printf("Value '0x%" PRIXMAX "' is greater than maximum allowed %" PRIXMAX "\r\n", value, UINT16_MAX);
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
	if (argc != 1) {
		printf("Command 'iowd' takes 1 argument\r\n");
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
		printf("Value '0x%" PRIXMAX "' is greater than maximum allowed %" PRIXMAX "\r\n", value, UINT32_MAX);
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
		printf("0x%" PRIX8 "\r\n", result);
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
		printf("0x%" PRIX16 "\r\n", result);
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
		printf("0x%" PRIX32 "\r\n", result);
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
	if (argc != 1) {
		printf("Command 'mmwb' takes 1 argument\r\n");
		return -1;
	}
	mem = strtoull(argv[0], NULL, 0);
	if (mem == ULLONG_MAX || *(argv[0]) == '-') {
		printf("Invalid memory address '%s'\r\n", argv[0]);
		return -1;
	}
	value = strtoull(argv[1], NULL, 0);
	if (value > UINT8_MAX) {
		printf("Value '0x%" PRIXMAX "' is greater than maximum allowed %" PRIXMAX "\r\n", value, UINT8_MAX);
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
	if (argc != 1) {
		printf("Command 'mmww' takes 1 argument\r\n");
		return -1;
	}
	mem = strtoull(argv[0], NULL, 0);
	if (mem == ULLONG_MAX || *(argv[0]) == '-') {
		printf("Invalid memory address '%s'\r\n", argv[0]);
		return -1;
	}
	value = strtoull(argv[1], NULL, 0);
	if (value > UINT16_MAX) {
		printf("Value '0x%" PRIXMAX "' is greater than maximum allowed %" PRIXMAX "\r\n", value, UINT16_MAX);
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
	if (argc != 1) {
		printf("Command 'mmwd' takes 1 argument\r\n");
		return -1;
	}
	mem = strtoull(argv[0], NULL, 0);
	if (mem == ULLONG_MAX || *(argv[0]) == '-') {
		printf("Invalid memory address '%s'\r\n", argv[0]);
		return -1;
	}
	value = strtoull(argv[1], NULL, 0);
	if (value > UINT32_MAX) {
		printf("Value '0x%" PRIXMAX "' is greater than maximum allowed %" PRIXMAX "\r\n", value, UINT32_MAX);
		return -1;
	}
	check = mmio_mmwd(mem, (uint32_t)value);
	if (check != 0) {
		printf("mmwd error occured\r\n");
		return -1;
	}
	return 0;
}

