#include <unistd.h>
#include <string.h>
#include "esc.h"

enum esc_sequence esc_get_esc_sequence(void)
{
	int current_position = 1;
	int check = 0;
	char ch = 0;
	char buffer[TASK_MMIO_ESC_MAX_LENGTH] = {27};
	while (current_position < TASK_MMIO_ESC_MAX_LENGTH - 1) {
		check = read(1, &ch, 1);
		if (check == 0) {
			break;
		}
		buffer[current_position] = ch;
		current_position++;
	}
	buffer[current_position] = 0;
	if (0 == strcmp(buffer, "\033")) {
		return ESC_KEY;
	}
	if (0 == strcmp(buffer, TASK_MMIO_ESC_CURSOR_UP)) {
		return ESC_ARROW_UP;
	}
	if (0 == strcmp(buffer, TASK_MMIO_ESC_CURSOR_DOWN)) {
		return ESC_ARROW_DOWN;
	}
	if (0 == strcmp(buffer, TASK_MMIO_ESC_CURSOR_RIGHT)) {
		return ESC_ARROW_RIGHT;
	}
	if (0 == strcmp(buffer, TASK_MMIO_ESC_CURSOR_LEFT)) {
		return ESC_ARROW_LEFT;
	}
	return ESC_UNSUPPORTED;
}

