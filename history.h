#ifndef TASK_MMIO_HISTORY_H
#define TASK_MMIO_HISTORY_H

#include <stddef.h>

enum {
	HISTORY_MAX_NODE_COUNT = 128
};

struct history_node {
	struct history_node *prev_node;
	struct history_node *next_node;
	char *input;
};

struct history {
	struct history_node *tail_node;
	struct history_node *head_node;
	size_t node_count;
};

void history_node_init(struct history_node *node);

void history_node_set_input(struct history_node *node, const char *input);

void history_node_free(struct history_node *node);

void history_init(struct history *hist);

void history_add(struct history *hist, const char *input);

void history_print(const struct history *hist);

void history_free(struct history *hist);

#endif

