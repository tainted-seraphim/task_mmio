#include <stdio.h>
#include <stdlib.h>
#include <termios.h>

#include "esc.h"
#include "terminal.h"

static struct termios original_terminal;
static struct termios raw_terminal;

void terminal_init(void)
{
	tcgetattr(0, &original_terminal);
	raw_terminal = original_terminal;
	terminal_enable_raw_terminal(&raw_terminal);
	tcsetattr(0, TCSAFLUSH, &raw_terminal);
	atexit(terminal_reset);
	printf(TASK_MMIO_ESC_SCREEN_SAVE);
	printf(TASK_MMIO_ESC_SCREEN_ENABLE_ALT);
	printf(TASK_MMIO_ESC_CURSOR_HOME);
	fflush(stdout);
}

void terminal_enable_raw_terminal(struct termios *terminal)
{
	if (terminal == NULL) {
		return;
	}
	tcgetattr(0, terminal);
	terminal->c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
	terminal->c_oflag &= ~(OPOST);
	terminal->c_cflag &= (CS8);
	terminal->c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
	terminal->c_cc[VMIN] = 0;
	terminal->c_cc[VTIME] = 1;
}

void terminal_reset(void)
{
	tcsetattr(0, TCSAFLUSH, &original_terminal);
	raw_terminal = original_terminal;
	printf(TASK_MMIO_ESC_SCREEN_DISABLE_ALT);
	printf(TASK_MMIO_ESC_SCREEN_RESTORE);
	fflush(stdout);
}

