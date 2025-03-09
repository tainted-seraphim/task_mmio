/* Date: 09.03.2025 */

#ifndef TASK_MMIO_COMMAND_H
#define TASK_MMIO_COMMAND_H

struct command {
	char *command;
	char **arguments;
	size_t argument_count;
};

void command_init(struct command *cmd);

void command_string_to_command(const char *str, struct command *cmd);

void command_free(struct command *cmd);

#endif

