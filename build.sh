#!/bin/bash

# 1. Porządki
echo "--- Czyszczenie ---"
rm -f initrd.img
rm -rf rootfs # Lepiej wywalić i stworzyć na nowo
mkdir -p rootfs/{dev,proc,sys,bin,mnt}

# 2. Kompilacja
echo "--- Kompilacja QuneOs (Modular) ---"
# Skompiluj prosto do folderu rootfs pod nazwą init
if gcc -static -I./src $(find src -name "*.c") -o rootfs/init; then
    echo -e "\e[1;32mKompilacja udana!\e[0m"
else
    echo -e "\e[1;31mBŁĄD KOMPILACJI!\e[0m"
    exit 1
fi

# 3. Flaga -static (WAŻNE!)
# Dodaj powyżej flagę -static w gcc, bo Twój system nie ma bibliotek .so w /lib

# 4. Pakowanie
echo "--- Pakowanie initramfs ---"
cd rootfs
find . -print0 | cpio --null -ov --format=newc --quiet | gzip -9 > ../initrd.img
cd ..

# 5. Start
echo "--- Launching QuneOS ---"
# console
# qemu-system-x86_64 -kernel linux/arch/x86/boot/bzImage -initrd initrd.img -append "console=ttyS0 quiet rdinit=/init" -nographic -hda storage.img
# qemu win
qemu-system-x86_64 -kernel linux/arch/x86/boot/bzImage -initrd initrd.img -append "quiet rdinit=/init" -hda storage.img

# clear
~/logo.sh
