#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "history.h"

static int get_max_number_length(size_t number);

void history_add(struct history *hist, const char *input)
{
	struct history_node *new_node = NULL;
	struct history_node *temp = NULL;

	if (hist == NULL || input == NULL) {
		return;
	}

	new_node = (struct history_node *)malloc(sizeof(struct history_node));
	if (new_node == NULL) {
		printf("history_add new_node malloc failed\r\n");
		exit(1);
	}
	history_node_init(new_node);
	history_node_set_input(new_node, input);
	if (hist->node_count == 0) {
		new_node->prev_node = hist->tail_node;
		new_node->next_node = hist->head_node;
		hist->tail_node->next_node = new_node;
		hist->head_node->prev_node = new_node;
		hist->node_count = 1;
	} else if (hist->node_count == HISTORY_MAX_NODE_COUNT) {
		temp = hist->tail_node->next_node;
		hist->tail_node->next_node = temp->next_node;
		temp->next_node->prev_node = hist->tail_node;
		history_node_free(temp);

		temp = hist->head_node->prev_node;
		temp->next_node = new_node;
		hist->head_node->prev_node = new_node;
		new_node->prev_node = temp->next_node;
		new_node->next_node = hist->head_node;
	} else {
		temp = hist->head_node->prev_node;
		new_node->prev_node = temp;
		new_node->next_node = hist->head_node;
		temp->next_node = new_node;
		hist->head_node->prev_node = new_node;
		hist->node_count++;
	}
	temp = NULL;
	new_node = NULL;
}

void history_free(struct history *hist)
{
	struct history_node *curr_node = NULL;
	struct history_node *next_node = NULL;

	if (hist == NULL) {
		return;
	}

	curr_node = hist->tail_node;
	if (curr_node == NULL) {
		return;
	}

	while (curr_node != NULL) {
		next_node = curr_node->next_node;
		history_node_free(curr_node);
		free(curr_node);
		curr_node = next_node;
	}
	hist->tail_node = NULL;
	hist->head_node = NULL;
	hist->node_count = 0;
}

void history_init(struct history *hist)
{
	if (hist == NULL) {
		return;
	}

	hist->head_node = (struct history_node *)
	                  malloc(sizeof(struct history_node));
	if (hist->head_node == NULL) {
		printf("history_init head_node malloc failed\r\n");
		exit(1);
	}
	hist->tail_node = (struct history_node *)
	                  malloc(sizeof(struct history_node));
	if (hist->tail_node == NULL) {
		printf("history_init tail_node malloc failed\r\n");
		exit(1);
	}
	history_node_init(hist->tail_node);
	history_node_init(hist->head_node);
	hist->tail_node->next_node = hist->head_node;
	hist->head_node->prev_node = hist->tail_node;
	hist->node_count = 0;
}

/* This function should not be called by the user, only by history_free */
/* history_free is responsible for freeing prev_node and next_node      */
void history_node_free(struct history_node *node)
{
	if (node == NULL) {
		return;
	}
	node->prev_node = NULL;
	node->next_node = NULL;
	free(node->input);
	node->input = NULL;
}

void history_node_init(struct history_node *node)
{
	if (node == NULL) {
		return;
	}
	node->prev_node = NULL;
	node->next_node = NULL;
	node->input = NULL;
}

void history_node_set_input(struct history_node *node, const char *input)
{
	size_t len = 0;
	if (node == NULL || input == NULL) {
		return;
	}
	len = strlen(input);
	if (node->input != NULL) {
		free(node->input);
	}
	node->input = (char *)malloc(len + 1);
	if (node->input == NULL) {
		printf("history_node_set_input node->input malloc failed\r\n");
		exit(1);
	}
	memmove(node->input, input, len);
	node->input[len] = 0;
}

void history_print(const struct history *hist)
{
	struct history_node *node = NULL;
	size_t index = 1;
	int depth = 0;
	if (hist == NULL) {
		return;
	}
	if (hist->node_count == 0) {
		return;
	}
	depth = get_max_number_length(hist->node_count);
	node = hist->tail_node->next_node;
	while (node != hist->head_node) {
		printf("%*zu  %s\r\n", depth, index, node->input);
		index++;
		node = node->next_node;
	}
}

static int get_max_number_length(size_t number)
{
	int len = 0;
	if (number == 0) {
		return 1;
	}
	while (number) {
		len++;
		number /= 10;
	}
	return len;
}

