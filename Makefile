SRC = src
BIN = bin

SOURCES = $(shell find $(SRC) -name "*.c")
OBJS = $(patsubst $(SRC)/%.c, $(BIN)/%.o, $(SOURCES))

CC = gcc
CFLAGS = -m32 -ffreestanding -fno-pie -fno-stack-protector -nostdlib -I$(SRC) -std=gnu99

all: $(BIN)/os-image.bin

$(BIN)/bootloader.bin: $(SRC)/boot/bootloader.asm
	@mkdir -p $(dir $@)
	nasm -f bin $< -o $@

$(BIN)/%.o: $(SRC)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN)/kernel.bin: $(OBJS)
	ld -m elf_i386 -T linker.ld -nostdlib --oformat binary -S $^ -o $@

$(BIN)/os-image.bin: $(BIN)/bootloader.bin $(BIN)/kernel.bin
	dd if=/dev/zero of=$(BIN)/os-image.bin bs=512 count=2880
	dd if=$(BIN)/bootloader.bin of=$(BIN)/os-image.bin conv=notrunc
	dd if=$(BIN)/kernel.bin of=$(BIN)/os-image.bin seek=1 conv=notrunc

run: all
	qemu-system-i386 -drive format=raw,file=$(BIN)/os-image.bin,if=floppy -vga std -m 256M

clean:
	rm -rf $(BIN)
