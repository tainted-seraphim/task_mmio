/* Date: 09.03.2025 */

#ifndef TASK_MMIO_HISTORY_H
#define TASK_MMIO_HISTORY_H

#include <stddef.h>

#define HISTORY_MAX_NODE_COUNT 128ULL

struct history_node {
	struct history_node *prev_node;
	struct history_node *next_node;
	char *input;
};

struct history {
	struct history_node *tail_node;
	struct history_node *head_node;
	struct history_node *current_node;
	size_t node_count;
};

void history_node_init(struct history_node *node);

void history_node_set_input(struct history_node *node, const char *input);

void history_node_free(struct history_node *node);

void history_init(struct history *hist);

void history_add(struct history *hist, const char *input);

void history_print(const struct history *hist);

void history_free(struct history *hist);

void history_current_node_up(struct history *hist);

void history_current_node_down(struct history *hist);

#endif

