#ifndef TASK_MMIO_ESC_H
#define TASK_MMIO_ESC_H

#define TASK_MMIO_ESC_SCREEN_ENABLE_ALT  "\033[?1049h"
#define TASK_MMIO_ESC_SCREEN_DISABLE_ALT "\033[?1049l"
#define TASK_MMIO_ESC_SCREEN_SAVE        "\033[47h"
#define TASK_MMIO_ESC_SCREEN_RESTORE     "\033[47l"
#define TASK_MMIO_ESC_SCREEN_CLEAR       "\033[2J"
#define TASK_MMIO_ESC_SCREEN_TOP         "\033[0;0H"
#define TASK_MMIO_ESC_LINE_RESET         "\033[2K\r"
#define TASK_MMIO_ESC_CURSOR_UP          "\033[A"
#define TASK_MMIO_ESC_CURSOR_DOWN        "\033[B"
#define TASK_MMIO_ESC_CURSOR_RIGHT       "\033[C"
#define TASK_MMIO_ESC_CURSOR_LEFT        "\033[D"
#define TASK_MMIO_ESC_CURSOR_HOME        "\033[H"
#define TASK_MMIO_ESC_CURSOR_END         "\033[F"

#define TASK_MMIO_ESC_MAX_LENGTH         64ULL

enum esc_sequence {
	ESC_KEY,
	ESC_ARROW_UP,
	ESC_ARROW_DOWN,
	ESC_ARROW_RIGHT,
	ESC_ARROW_LEFT,
	ESC_HOME,
	ESC_END,
	ESC_UNSUPPORTED
};

enum esc_sequence esc_get_esc_sequence(void);

#endif

