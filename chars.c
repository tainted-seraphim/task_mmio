#include <unistd.h>
#include <string.h>
#include "chars.h"
#include "esc.h"

enum char_type chars_get_char_type(char ch)
{
	switch (ch) {
	case 0:
		return CHAR_IDLE;
	case 3:
		return CHAR_QUIT;
	case 4:
		return CHAR_INTERRUPT;
	case 10:
	case 13:
		return CHAR_NEWLINE;
	case 8:
	case 127:
		return CHAR_BACKSPACE;
	case 27:
		return chars_get_esc_sequence();
	default:
		if (ch >= 32 && ch <= 126) {
			return CHAR_PRINTABLE;
		}
		return CHAR_UNSUPPORTED;
	}
}

enum char_type chars_get_esc_sequence(void)
{
	char buff[8] = {27};
	read(1, buff + 1, 7);
	buff[7] = '\0';
	if (0 == strcmp(buff, TASK_MMIO_ESC_CURSOR_UP)) {
		return CHAR_ARROW_UP;
	}
	if (0 == strcmp(buff, TASK_MMIO_ESC_CURSOR_DOWN)) {
		return CHAR_ARROW_DOWN;
	}
	if (0 == strcmp(buff, TASK_MMIO_ESC_CURSOR_RIGHT)) {
		return CHAR_ARROW_RIGHT;
	}
	if (0 == strcmp(buff, TASK_MMIO_ESC_CURSOR_LEFT)) {
		return CHAR_ARROW_LEFT;
	}
	return CHAR_UNSUPPORTED;
}

