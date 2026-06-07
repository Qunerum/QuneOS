SRC = src
BIN = bin

SOURCES_C = $(shell find $(SRC) -name "*.c")
SOURCES_ASM = $(filter-out $(SRC)/boot/bootloader.asm, $(shell find $(SRC) -name "*.asm"))

OBJS_C = $(patsubst $(SRC)/%.c, $(BIN)/%.o, $(SOURCES_C))
OBJS_ASM = $(patsubst $(SRC)/%.asm, $(BIN)/%.o, $(SOURCES_ASM))
OBJS = $(OBJS_C) $(OBJS_ASM)

CC = gcc
CFLAGS = -m32 -ffreestanding -O2 -fno-pie -fno-stack-protector -fno-asynchronous-unwind-tables -nostdlib -I$(SRC) -std=gnu99

all: $(BIN)/os-image.bin

$(BIN)/bootloader.bin: $(SRC)/boot/bootloader.asm
	@mkdir -p $(dir $@)
	nasm -f bin $< -o $@

$(BIN)/%.o: $(SRC)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN)/%.o: $(SRC)/%.asm
	@mkdir -p $(dir $@)
	nasm -f elf32 $< -o $@

$(BIN)/kernel.bin: $(OBJS)
	ld -m elf_i386 -T linker.ld -nostdlib --oformat binary -S $^ -o $@

$(BIN)/os-image.bin: $(BIN)/bootloader.bin $(BIN)/kernel.bin
	dd if=/dev/zero of=$(BIN)/os-image.bin bs=512 count=4000
	dd if=$(BIN)/bootloader.bin of=$(BIN)/os-image.bin conv=notrunc
	dd if=$(BIN)/kernel.bin of=$(BIN)/os-image.bin seek=1 conv=notrunc

run: all
	qemu-system-i386 -drive format=raw,file=$(BIN)/os-image.bin,if=ide -vga std -m 256M

clean:
	rm -rf $(BIN)
