CC = gcc
CFLAGS = -std=c99 -pedantic -g -Wall -Wextra
TARGET = mmio_io
OBJECT = chars.o\
	 print.o\
	 history.o\
	 command.o\
	 console.o\
	 terminal.o\
	 mmio.o\
	 esc.o

release: CFLAGS = -std=c99 -pedantic -Wall -Wextra
release: $(TARGET)

debug: $(TARGET)

$(TARGET): main.c $(OBJECT)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(TARGET)
	rm -rf $(OBJECT)

chars.c: chars.h esc.h
print.c: print.h esc.h
history.c: history.h
command.c: command.h
console.c: console.h command.h history.h chars.h print.h mmio.h
terminal.c: terminal.h esc.h
mmio.c: mmio.h
esc.c: esc.h

