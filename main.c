#include "console.h"
#include "terminal.h"

int main(void)
{
	struct console con;
	console_init(&con);
	terminal_init();
	console_run(&con);
	terminal_reset();
	console_free(&con);
	return 0;
}

