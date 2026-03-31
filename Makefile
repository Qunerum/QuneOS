CC = gcc
AS = nasm
LD = ld

LDFLAGS = -m elf_i386 -T linker.ld
CFLAGS = -m32 -ffreestanding -fno-stack-protector -nostdlib -I.

OBJ = bin/boot.o \
      bin/kernel.o \
      bin/utility.o \
      bin/terminal.o \
      bin/terminalCMDs.o \
      bin/files.o

BIN = bin/qos-kernel.bin

all: $(BIN)

bin/boot.o: src/boot/boot.asm
	mkdir -p bin
	$(AS) -f elf32 src/boot/boot.asm -o bin/boot.o

bin/%.o: src/kernel/%.c
	mkdir -p bin
	$(CC) $(CFLAGS) -c $< -o $@

bin/%.o: src/common/%.c
	mkdir -p bin
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN): $(OBJ)
	$(LD) $(LDFLAGS) $(OBJ) -o $(BIN)

run: $(BIN)
	qemu-system-i386 -kernel $(BIN) -drive file=disk.img,format=raw,if=ide -net none

clean:
	rm -rf bin/
