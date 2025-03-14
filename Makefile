# 13.03.2025

CC      = gcc
CFLAGS  = -std=c99 -pedantic -Wall
TARGET  = mmio_io
SOURCES = chars.c command.c console.c esc.c history.c input.c main.c mmio.c print.c terminal.c
OBJECTS = $(SOURCES:%.c=%.o)
DEPS    = $(SOURCES:%.c=%.d)

all: release

release: $(TARGET)

debug: CFLAGS = -std=c99 -pedantic -g -Wall -Wextra
debug: $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.d: %.c
	$(CC) -MM $< -MF $@

include $(DEPS)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f $(TARGET)
	rm -f $(OBJECTS)
	rm -f $(DEPS)


