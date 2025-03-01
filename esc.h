#ifndef TASK_MMIO_ESC_H
#define TASK_MMIO_ESC_H

#define TASK_MMIO_ESC_SCREEN_ENABLE_ALT  "\033[?1049h"
#define TASK_MMIO_ESC_SCREEN_DISABLE_ALT "\033[?1049l"
#define TASK_MMIO_ESC_SCREEN_SAVE        "\033[47h"
#define TASK_MMIO_ESC_SCREEN_RESTORE     "\033[47l"
#define TASK_MMIO_ESC_SCREEN_CLEAR       "\033[2J\033[H"
#define TASK_MMIO_ESC_LINE_RESET         "\033[2K\r"
#define TASK_MMIO_ESC_CURSOR_UP          "\033[A"
#define TASK_MMIO_ESC_CURSOR_DOWN        "\033[B"
#define TASK_MMIO_ESC_CURSOR_RIGHT       "\033[C"
#define TASK_MMIO_ESC_CURSOR_LEFT        "\033[D"
#define TASK_MMIO_ESC_CURSOR_HOME        "\033[H"

#endif

