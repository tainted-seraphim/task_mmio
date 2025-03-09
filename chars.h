/* Date: 09.03.2025 */

#ifndef TASK_MMIO_CHARS_H
#define TASK_MMIO_CHARS_H

enum char_type {
	CHAR_IDLE,
	CHAR_PRINTABLE,
	CHAR_NEWLINE,
	CHAR_BACKSPACE,
	CHAR_ESC,
	CHAR_INTERRUPT,
	CHAR_QUIT,
	CHAR_UNSUPPORTED
};

enum char_type chars_get_char_type(char ch);

#endif

