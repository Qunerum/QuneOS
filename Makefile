SRC = src
BIN = bin

# 1. Automatycznie znajdź pliki .c oraz .asm wewnątrz jądra/driverów
SOURCES_C = $(shell find $(SRC) -name "*.c")
# Szukamy plików .asm, ale OMIJAMY bootloader, bo on ma osobną regułę kompilacji na plik płaski (.bin)
SOURCES_ASM = $(filter-out $(SRC)/boot/bootloader.asm, $(shell find $(SRC) -name "*.asm"))

# 2. Zamień ścieżki plików źródłowych na ścieżki plików obiektowych (.o)
OBJS_C = $(patsubst $(SRC)/%.c, $(BIN)/%.o, $(SOURCES_C))
OBJS_ASM = $(patsubst $(SRC)/%.asm, $(BIN)/%.o, $(SOURCES_ASM))
OBJS = $(OBJS_C) $(OBJS_ASM)

CC = gcc
CFLAGS = -m32 -ffreestanding -fno-pie -fno-stack-protector -nostdlib -I$(SRC) -std=gnu99

all: $(BIN)/os-image.bin

# Reguła dla bootloadera (tworzy czysty plik binarny 512 bajtów)
$(BIN)/bootloader.bin: $(SRC)/boot/bootloader.asm
	@mkdir -p $(dir $@)
	nasm -f bin $< -o $@

# Reguła dla kompilacji plików .c
$(BIN)/%.o: $(SRC)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# NOWA REGUŁA: Kompilacja plików .asm należących do jądra do formatu ELF (32-bit)
$(BIN)/%.o: $(SRC)/%.asm
	@mkdir -p $(dir $@)
	nasm -f elf32 $< -o $@

# Linker sklei teraz pliki .o powstałe z C ORAZ z Assemblera
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
