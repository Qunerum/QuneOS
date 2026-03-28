CC = gcc
AS = nasm
LD = ld

LDFLAGS = -m elf_i386 -T linker.ld
CFLAGS = -m32 -ffreestanding -fno-stack-protector -nostdlib -I.

OBJ = bin/boot.o \
      bin/kernel.o \
      bin/utility.o

BIN = bin/qos-kernel.bin

all: $(BIN)

bin/boot.o: boot/boot.asm
	mkdir -p bin
	$(AS) -f elf32 boot/boot.asm -o bin/boot.o

bin/%.o: kernel/%.c
	mkdir -p bin
	$(CC) $(CFLAGS) -c $< -o $@

bin/%.o: common/%.c
	mkdir -p bin
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN): $(OBJ)
	$(LD) $(LDFLAGS) $(OBJ) -o $(BIN)

run: $(BIN)
	qemu-system-i386 -kernel $(BIN) -net none

clean:
	rm -rf bin/
