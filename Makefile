SRC = src
OBJ = obj

SOURCES_C = $(shell find $(SRC) -name "*.c")
SOURCES_ASM = $(filter-out $(SRC)/boot/bootloader.asm, $(shell find $(SRC) -name "*.asm"))

OBJS_C = $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SOURCES_C))
OBJS_ASM = $(patsubst $(SRC)/%.asm, $(OBJ)/%.o, $(SOURCES_ASM))
OBJS = $(OBJS_C) $(OBJS_ASM)

CC = gcc
CFLAGS = -m32 -ffreestanding -O2 -fno-pie -fno-stack-protector -fno-asynchronous-unwind-tables -nostdlib -I$(SRC) -std=gnu99

all: $(OBJ)/os-image.bin

$(OBJ)/bootloader.bin: $(SRC)/boot/bootloader.asm
	@mkdir -p $(dir $@)
	nasm -f bin $< -o $@

$(OBJ)/%.o: $(SRC)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ)/%.o: $(SRC)/%.asm
	@mkdir -p $(dir $@)
	nasm -f elf32 $< -o $@

$(OBJ)/kernel.bin: $(OBJS)
	ld -m elf_i386 -T linker.ld -nostdlib --oformat binary -S $^ -o $@

$(OBJ)/os-image.bin: $(OBJ)/bootloader.bin $(OBJ)/kernel.bin
	dd if=/dev/zero of=$(OBJ)/os-image.bin bs=512 count=4000
	dd if=$(OBJ)/bootloader.bin of=$(OBJ)/os-image.bin conv=notrunc
	dd if=$(OBJ)/kernel.bin of=$(OBJ)/os-image.bin seek=1 conv=notrunc

run: all
	qemu-system-i386 -drive format=raw,file=$(OBJ)/os-image.bin,if=ide -vga std -m 256M

clean:
	rm -rf $(OBJ)
