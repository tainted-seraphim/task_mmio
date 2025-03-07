#ifndef TASK_MMIO_INPUT_H
#define TASK_MMIO_INPUT_H

#include <stddef.h>

#define TASK_MMIO_INPUT_ALLOCATED_BYTES     32LLU
#define TASK_MMIO_INPUT_MAX_INPUT_LENGTH    (TASK_MMIO_INPUT_ALLOCATED_BYTES - 1)
#define TASK_MMIO_INPUT_MAX_CURSOR_POSITION TASK_MMIO_INPUT_MAX_INPUT_LENGTH

/* buffer is null-terminated string                           */
/* Every symbol from first \0 to the end of buffer is also \0 */
/* input_length = strlen(buffer)                              */
/* input_length ~ [0; TASK_MMIO_INPUT_MAX_INPUT_LENGTH]       */
/* cursor_position tells where to set / insert / delete       */
/* cursor_position can't be greater than input_length         */
/* cursor_position ~ [0; TASK_MMIO_INPUT_MAX_CURSOR_POSITION] */
struct input {
	char buffer[TASK_MMIO_INPUT_ALLOCATED_BYTES];
	size_t input_length;
	size_t cursor_position;
};

void input_init(struct input *input);

void input_clear(struct input *input);

void input_set_buffer(struct input *input, const char *str);

void input_set_cursor_position(struct input *input, size_t pos);

void input_auto_set_fields(struct input *input);

void input_copy_input(struct input *dest_input, const struct input *src_input);

void input_free(struct input *input);

void input_insert_byte(struct input *input, char byte);

void input_backspace(struct input *input);

void input_delete(struct input *input);

void input_move_cursor_right(struct input *input);

void input_move_cursor_left(struct input *input);

void input_move_cursor_home(struct input *input);

void input_move_cursor_end(struct input *input);

#endif

