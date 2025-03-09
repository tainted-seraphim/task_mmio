/* Date: 09.03.2025 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>

#include "esc.h"
#include "terminal.h"

struct termios *base_terminal = NULL;

void terminal_init(struct termios *terminal)
{
	if (terminal == NULL) {
		return;
	}

	if (base_terminal == NULL) {
		base_terminal = (struct termios *)malloc(sizeof(struct termios));
		if (base_terminal == NULL) {
			printf("terminal_init base_terminal malloc failed\r\n");
			exit(1);
		}
		tcgetattr(0, base_terminal);
	}
	memmove(terminal, base_terminal, sizeof(struct termios));
}

void terminal_free(struct termios *terminal)
{
	if (terminal == NULL) {
		return;
	}

	memset(terminal, 0, sizeof(struct termios));
}

void terminal_enable_raw_terminal(struct termios *terminal)
{
	if (terminal == NULL) {
		return;
	}

	tcgetattr(0, terminal);
	terminal->c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
	terminal->c_oflag &= ~(OPOST);
	terminal->c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
	terminal->c_cflag &= ~(CSIZE | PARENB);
	terminal->c_cflag |= (CS8);
	terminal->c_cc[VMIN] = 0;
	terminal->c_cc[VTIME] = 1;
}

void terminal_disable_raw_terminal(struct termios *terminal)
{
	if (terminal == NULL) {
		return;
	}

	memmove(terminal, base_terminal, sizeof(struct termios));
}

void terminal_set_terminal(struct termios *terminal)
{
	if (terminal == NULL) {
		return;
	}

	tcsetattr(0, TCSAFLUSH, terminal);
}

