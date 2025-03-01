CC = gcc
CFLAGS = -std=c99 -pedantic -g -Wall -Wextra
TARGET = mmio_io
OBJECT = main.o\
	 chars.o\
	 print.o\
	 history.o\
	 command.o\
	 console.o\
	 terminal.o\
	 mmio.o

$(TARGET): $(OBJECT)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(TARGET)
	rm -rf $(OBJECT)

