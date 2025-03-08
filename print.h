#ifndef TASK_MMIO_PRINT_H
#define TASK_MMIO_PRINT_H

#include <stddef.h>
#define TASK_MMIO_INPUT_PROMPT "> "

void print_header(void);

void print_help(void);

void print_input_prompt(void);

void print_line_reset(void);

void print_move_cursor_left(void);

void print_move_cursor_right(void);

void print_set_cursor_position(size_t pos);

void print_newline(void);

void print_screen_clear(void);

void print_enable_alt_screen(void);

void print_disable_alt_screen(void);

#endif

