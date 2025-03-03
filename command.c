#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "command.h"

void command_free(struct command *cmd)
{
	size_t index = 0;
	if (cmd == NULL) {
		return;
	}
	free(cmd->command);
	cmd->command = NULL;
	for (index = 0; index < cmd->argument_count; index++) {
		free(cmd->arguments[index]);
		cmd->arguments[index] = NULL;
	}
	free(cmd->arguments);
	cmd->arguments = NULL;
	cmd->argument_count = 0;
}

void command_init(struct command *cmd)
{
	if (cmd == NULL) {
		return;
	}
	cmd->command = NULL;
	cmd->arguments = NULL;
	cmd->argument_count = 0;
}

void command_string_to_command(const char *str, struct command *cmd)
{
	char *temp_str = NULL;
	char *token = NULL;
	size_t count = 0;
	size_t len = 0;
	if (cmd == NULL || str == NULL || *str == 0) {
		return;
	}
	command_free(cmd);
	len = strlen(str);
	temp_str = (char *)malloc(len + 1);
	if (temp_str == NULL) {
		printf("command_string_to_command temp_str malloc failed\r\n");
		exit(1);
	}
	memmove(temp_str, str, len + 1);
	token = strtok(temp_str, " ");
	if (token == NULL) {
		free(temp_str);
		return;
	}
	len = strlen(token);
	cmd->command = (char *)malloc(len + 1);
	if (cmd->command == NULL) {
		printf("command_string_to_command cmd->command malloc failed\r\n");
		exit(1);
	}
	memmove(cmd->command, token, len + 1);
	while ((token = strtok(NULL, " ")) != NULL) {
		len = strlen(token);
		cmd->arguments = (char **)
		                 realloc(cmd->arguments, sizeof(char *) * (count + 1));
		if (cmd->arguments == NULL) {
			printf("command_string_to_command cmd->arguments realloc failed\r\n");
			exit(1);
		}
		cmd->arguments[count] = (char *)malloc(len + 1);
		memmove(cmd->arguments[count], token, len + 1);
		count++;
	}
	cmd->argument_count = count;
	free(temp_str);
}

