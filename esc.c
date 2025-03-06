#include <unistd.h>
#include <string.h>
#include "esc.h"

enum esc_sequence esc_get_esc_sequence(void)
{
	int check = 0;
	char buffer[TASK_MMIO_ESC_MAX_LENGTH] = {27};
	/* This enables smooth arrow movement and history scrolling */
	check = read(1, buffer + 1, TASK_MMIO_ESC_MAX_LENGTH - 1);
	if (check == 0 || buffer[1] == 27) {
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
	if (0 == strcmp(buffer, TASK_MMIO_ESC_CURSOR_HOME)) {
		return ESC_HOME;
	}
	if (0 == strcmp(buffer, TASK_MMIO_ESC_CURSOR_END)) {
		return ESC_END;
	}
	return ESC_UNSUPPORTED;
}

