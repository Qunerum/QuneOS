#!/bin/bash

# 1. Porządki
echo "--- Czyszczenie ---"
rm -f initrd.img
rm -f rootfs/init

# 2. Struktura
mkdir -p rootfs/{dev,proc,sys,bin}

# 3. Kompilacja wszystkich plików .c z folderu src/
echo "--- Kompilacja QuneOs (Modular) ---"
# gcc weźmie main.c, files.c i utils.c i połączy je w jeden plik 'init'
if gcc -static src/*.c -o rootfs/init; then
    echo -e "\e[1;32mKompilacja udana!\e[0m"
else
    echo -e "\e[1;31mBŁĄD KOMPILACJI!\e[0m"
    exit 1
fi

# 4. Pakowanie
echo "--- Pakowanie initramfs ---"
cd rootfs
find . -print0 | cpio --null -ov --format=newc --quiet | gzip -9 > ../initrd.img
cd ..

# 5. Start
echo "--- Launching QuneOS in a new terminal window ---"
# qemu-system-x86_64 -kernel linux/arch/x86/boot/bzImage -initrd initrd.img -append "console=ttyS0 quiet rdinit=/init" -nographic -hda storage.img
qemu-system-x86_64 -kernel linux/arch/x86/boot/bzImage -initrd initrd.img -append "quiet rdinit=/init" -hda storage.img

# clear
~/logo.sh
