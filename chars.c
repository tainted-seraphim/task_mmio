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
		return CHAR_ESC;
	default:
		if (ch >= 32 && ch <= 126) {
			return CHAR_PRINTABLE;
		}
		return CHAR_UNSUPPORTED;
	}
}

