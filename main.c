/* Date: 09.03.2025 */

#include "console.h"

int main(void)
{
	struct console con;
	console_init(&con);
	console_run(&con);
	console_free(&con);
	return 0;
}

