/* Date: 09.03.2025 */

#include <stdio.h>
#include <string.h>
#include "esc.h"
#include "print.h"

void print_header(void)
{
	const char *message =
	        "#     # #     # ### #######       # ### #######\r\n"
	        "##   ## ##   ##  #  #     #      #   #  #     #\r\n"
	        "# # # # # # # #  #  #     #     #    #  #     #\r\n"
	        "#  #  # #  #  #  #  #     #    #     #  #     #\r\n"
	        "#     # #     #  #  #     #   #      #  #     #\r\n"
	        "#     # #     #  #  #     #  #       #  #     #\r\n"
	        "#     # #     # ### ####### #       ### #######\r\n"
	        "\r\n"
	        "Type 'help' to get info about available commands\r\n"
	        "Type 'quit' or Ctrl+C to quit the program\r\n"
	        "Autocomplete is not implemented\r\n"
	        "If you kill this process, better reset the terminal!\r\n"
	        ;
	printf("%s\r\n", message);
}

void print_help(void)
{
	const char *message =
	        "'quit'            - exit the program\r\n"
	        "'help'            - get help\r\n"
	        "'history'         - print command history\r\n"
	        "'clear'           - clear the screen\r\n"
	        "'iorb' port       - read byte from port I/O\r\n"
	        "'iorw' port       - read word from port I/O\r\n"
	        "'iord' port       - read dword from port I/O\r\n"
	        "'iowb' port value - write byte to port I/O\r\n"
	        "'ioww' port value - write word to port I/O\r\n"
	        "'iowd' port value - write dword to port I/O\r\n"
	        "'mmrb' addr       - read byte from MMIO\r\n"
	        "'mmrw' addr       - read word from MMIO\r\n"
	        "'mmrd' addr       - read dword from MMIO\r\n"
	        "'mmwb' addr value - write byte to MMIO\r\n"
	        "'mmww' addr value - write word to MMIO\r\n"
	        "'mmwd' addr value - write dword to MMIO\r\n"
	        ;
	printf("\r\n%s\r\n", message);
}

void print_input_prompt(void)
{
	printf(TASK_MMIO_INPUT_PROMPT);
}

void print_line_reset(void)
{
	printf(TASK_MMIO_ESC_LINE_RESET);
}

void print_move_cursor_left(void)
{
	printf(TASK_MMIO_ESC_CURSOR_LEFT);
}

void print_move_cursor_right(void)
{
	printf(TASK_MMIO_ESC_CURSOR_RIGHT);
}

void print_newline(void)
{
	printf("\r\n");
}

void print_screen_clear(void)
{
	printf(TASK_MMIO_ESC_SCREEN_CLEAR);
	printf(TASK_MMIO_ESC_SCREEN_TOP);
}

void print_set_cursor_position(size_t pos)
{
	printf("\033[%zuG", strlen(TASK_MMIO_INPUT_PROMPT) + pos);
}

void print_enable_alt_screen(void)
{
	printf(TASK_MMIO_ESC_SCREEN_SAVE);
	printf(TASK_MMIO_ESC_SCREEN_ENABLE_ALT);
	printf(TASK_MMIO_ESC_SCREEN_CLEAR);
	printf(TASK_MMIO_ESC_SCREEN_TOP);	
}

void print_disable_alt_screen(void)
{
	printf(TASK_MMIO_ESC_SCREEN_CLEAR);
	printf(TASK_MMIO_ESC_SCREEN_TOP);
	printf(TASK_MMIO_ESC_SCREEN_DISABLE_ALT);
	printf(TASK_MMIO_ESC_SCREEN_RESTORE);
}

