/* Date: 09.03.2025 */

#include <stdlib.h>
#include <string.h>
#include "input.h"

void input_init(struct input *input)
{
	/* Initializes input */
	if (input == NULL) {
		return;
	}

	memset(input->buffer, 0, TASK_MMIO_INPUT_ALLOCATED_BYTES);
	input->input_length = 0;
	input->cursor_position = 0;
}

void input_clear(struct input *input)
{
	/* Nullifies input */
	if (input == NULL) {
		return;
	}

	memset(input->buffer, 0, TASK_MMIO_INPUT_ALLOCATED_BYTES);
	input->input_length = 0;
	input->cursor_position = 0;
}

void input_set_buffer(struct input *input, const char *str)
{
	/* Copies TASK_MMIO_INPUT_MAX_INPUT_LENGTH bytes of str  */
	/* If str has less bytes, rest of buffer is nullified    */
	size_t len = 0;

	if (input == NULL) {
		return;
	}
	if (str == NULL || *str == 0) {
		input_clear(input);
		return;
	}

	len = strlen(str);
	if (len > TASK_MMIO_INPUT_MAX_INPUT_LENGTH) {
		len = TASK_MMIO_INPUT_MAX_INPUT_LENGTH;
	}
	memmove(input->buffer, str, len);
	memset(input->buffer + len, 0, TASK_MMIO_INPUT_ALLOCATED_BYTES - len);
	input->input_length = len;
	input->cursor_position = len;
}

void input_set_cursor_position(struct input *input, size_t pos)
{
	/* Sets cursor_position with range check */
	if (input == NULL) {
		return;
	}

	if (pos > input->input_length) {
		input->cursor_position = input->input_length;
	} else {
		input->cursor_position = pos;
	}
}

void input_auto_set_fields(struct input *input)
{
	/* Sets cursor_position and input_length based on buffer contents */
	char *dest = NULL;
	size_t count = 0;
	size_t len = 0;

	if (input == NULL) {
		return;
	}

	input->buffer[TASK_MMIO_INPUT_MAX_INPUT_LENGTH] = 0;
	len = strlen(input->buffer);
	input->input_length = len;
	input->cursor_position = len;
	dest = input->buffer + input->cursor_position;
	count = TASK_MMIO_INPUT_MAX_CURSOR_POSITION + 1 - input->cursor_position;
	memset(dest, 0, count);
}

void input_copy_input(struct input *dest_input, const struct input *src_input)
{
	/* Copies object contents */
	if (dest_input == NULL || src_input == NULL) {
		return;
	}

	memmove(dest_input->buffer, src_input->buffer, TASK_MMIO_INPUT_ALLOCATED_BYTES);
	dest_input->input_length = src_input->input_length;
	dest_input->cursor_position = src_input->cursor_position;
}

void input_free(struct input *input)
{
	/* Destroys object contents */
	if (input == NULL) {
		return;
	}

	memset(input->buffer, 0, TASK_MMIO_INPUT_ALLOCATED_BYTES);
	input->input_length = 0;
	input->cursor_position = 0;
}

void input_insert_byte(struct input *input, char byte)
{
	/* Insert character where cursor is pointing                       */
	/* Shift buffer contents right for 1 position from cursor_position */
	/* Insert symbol 'byte' in cursor_position                         */
	/* Input wraps at position = TASK_MMIO_INPUT_MAX_INPUT_LENGTH      */
	/* If insertion was performed, cursor_position is incremented      */
	/* Unless resulting input wraps, input_length is incremented       */
	/* "abdefgh  " --> "abcdefgh "                                     */
	/*    ^     #          ^    #                                      */
	/*    c                                                            */
	char *dest = NULL;
	char *src = NULL;
	size_t count = 0;

	if (input == NULL) {
		return;
	}
	if (input->cursor_position == TASK_MMIO_INPUT_MAX_CURSOR_POSITION) {
		return;
	}

	if (byte == 0) {
		dest = input->buffer + input->cursor_position;
		count = TASK_MMIO_INPUT_MAX_CURSOR_POSITION + 1 - input->cursor_position;
		memset(dest, 0, count);
		input->input_length = input->cursor_position;
		return;
	}
	dest = input->buffer + input->cursor_position + 1;
	src = input->buffer + input->cursor_position;
	count = TASK_MMIO_INPUT_MAX_CURSOR_POSITION - input->cursor_position;
	memmove(dest, src, count);
	input->buffer[input->cursor_position] = byte;
	input->buffer[TASK_MMIO_INPUT_MAX_INPUT_LENGTH] = 0;
	input->cursor_position++;
	if (input->input_length != TASK_MMIO_INPUT_MAX_INPUT_LENGTH) {
		input->input_length++;
	}
}

void input_backspace(struct input *input)
{
	/* Delete the character before the cursor if character exists     */
	/* Shift buffer contents left for 1 position from cursor_position */
	/* cursor_position is decremented unless it was 0                 */
	/* input_length is decremented if character was deleted           */
	/* "abcdefgh " --> "abcefgh  "                                    */
	/*      ^   #          ^    #                                     */
	/*                                                                */
	char *dest = NULL;
	char *src = NULL;
	size_t count = 0;

	if (input == NULL) {
		return;
	}
	if (input->cursor_position == 0) {
		return;
	}

	dest = input->buffer + input->cursor_position - 1;
	src = input->buffer + input->cursor_position;
	count = TASK_MMIO_INPUT_MAX_CURSOR_POSITION + 1 - input->cursor_position;
	memmove(dest, src, count);
	input->buffer[TASK_MMIO_INPUT_MAX_INPUT_LENGTH] = 0;
	input->input_length--;
	input->cursor_position--;
}

void input_delete(struct input *input)
{
	/* Delete character pointed by the cursor if character exists         */
	/* Shift buffer contents left for 1 position from cursor_position + 1 */
	/* cursor_position is unchanged                                       */
	/* input_length is decremented if character was deleted               */
	/* "abcdefgh " --> "abcdfgh  "                                        */
	/*      ^   #           ^   #                                         */
	/*                                                                    */
	char *dest = NULL;
	char *src = NULL;
	size_t count = 0;

	if (input == NULL) {
		return;
	}
	if (input->cursor_position == TASK_MMIO_INPUT_MAX_CURSOR_POSITION) {
		return;
	}

	dest = input->buffer + input->cursor_position;
	src = input->buffer + input->cursor_position + 1;
	count = TASK_MMIO_INPUT_MAX_CURSOR_POSITION - input->cursor_position;
	memmove(dest, src, count);
	input->buffer[TASK_MMIO_INPUT_MAX_INPUT_LENGTH] = 0;
	input->input_length--;
}

void input_move_cursor_right(struct input *input)
{
	/* Moves cursor_position right unless it reached the end of current buffer */
	if (input == NULL) {
		return;
	}
	if (input->cursor_position == input->input_length) {
		return;
	}

	input->cursor_position++;
}

void input_move_cursor_left(struct input *input)
{
	/* Moves cursor_position left unless it's at the beginning of buffer */
	if (input == NULL) {
		return;
	}
	if (input->cursor_position == 0) {
		return;
	}

	input->cursor_position--;
}

void input_move_cursor_home(struct input *input)
{
	/* Moves cursor_position to beginning of buffer */
	if (input == NULL) {
		return;
	}

	input->cursor_position = 0;
}

void input_move_cursor_end(struct input *input)
{
	/* Moves cursor_position to the end of current buffer */
	if (input == NULL) {
		return;
	}

	input->cursor_position = input->input_length;
}

