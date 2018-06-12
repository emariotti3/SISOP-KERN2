CFLAGS := -std=c99 -Wall -Wextra -Wpedantic -m32 -fasm
CFLAGS_EXTRA := -g -O1 -ffreestanding
LIB = lib
SRCS_C := $(wildcard *.c) $(wildcard $(LIB)/*.c)
SRCS_AS := $(wildcard *.S)
HEADERS := $(wildcard *.h)

OBJS_C := $(patsubst lib/, , $(SRCS_C:.c=.o))
OBJS_AS := $(patsubst %.S, %.o, $(SRCS_AS))

TARGET = kern2
CC := gcc

all: $(TARGET)

$(OBJS_C): $(SRCS_C)

$(OBJS_AS): $(SRCS_AS)

%.o: %.S
	$(CC) $(CFLAGS) $(CFLAGS_EXTRA) -c $<

%.o : %.c $(HEADERS)
	$(CC) -c $(CFLAGS) $(CFLAGS_EXTRA) $< -o $@

$(LIB)/%.o : $(LIB)/%.c
		$(CC) -c $(CFLAGS) $< -o $@


$(TARGET): $(OBJS_C) $(OBJS_AS)
	ld -m elf_i386 -Ttext 0x100000 --entry=_start $^ -o $@
	# Verificar imagen Multiboot v1.
	grub-file --is-x86-multiboot $@

clean:
	rm -f kern2 *.o core *.asm

.PHONY: clean qemu qemu-gdb gdb

QEMU := qemu-system-i386 -serial mon:stdio
KERN ?= kern2
BOOT := -kernel $(KERN) $(QEMU_EXTRA)

qemu: $(KERN)
	$(QEMU) $(BOOT)

qemu-gdb: $(KERN)
	$(QEMU) -kernel kern2 -S -gdb tcp:127.0.0.1:7508 $(BOOT)

gdb:
	gdb -q -s kern2 -n -ex 'target remote 127.0.0.1:7508'
