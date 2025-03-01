#include <stdio.h>
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
	        "'iorb' addr       - read byte from /dev/port\r\n"
	        "'iorw' addr       - read word from /dev/port\r\n"
	        "'iord' addr       - read dword from /dev/port\r\n"
	        "'iowb' addr value - write byte to /dev/port\r\n"
	        "'ioww' addr value - write word to /dev/port\r\n"
	        "'iowd' addr value - write dword to /dev/port\r\n"
	        "'mmrb' addr       - read byte from /dev/mem\r\n"
	        "'mmrw' addr       - read word from /dev/mem\r\n"
	        "'mmrd' addr       - read dword from /dev/mem\r\n"
	        "'mmwb' addr value - write byte to /dev/mem\r\n"
	        "'mmww' addr value - write word to /dev/mem\r\n"
	        "'mmwd' addr value - write dword to /dev/mem\r\n"
	        ;
	printf("\r\n%s\r\n", message);
}

void print_input_prompt(void)
{
	printf(TASK_MMIO_INPUT_PROMPT);
	fflush(stdout);
}

void print_line_reset(void)
{
	printf(TASK_MMIO_ESC_LINE_RESET);
	fflush(stdout);
}

void print_move_cursor_left(void)
{
	printf(TASK_MMIO_ESC_CURSOR_LEFT);
	fflush(stdout);
}

void print_move_cursor_right(void)
{
	printf(TASK_MMIO_ESC_CURSOR_RIGHT);
	fflush(stdout);
}

void print_newline(void)
{
	printf("\r\n");
}

void print_screen_clear(void)
{
	printf(TASK_MMIO_ESC_SCREEN_CLEAR);
	fflush(stdout);
}

