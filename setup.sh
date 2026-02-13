#!/bin/bash

# Colors for output
GREEN='\033[0;32m'
BLUE='\033[0;34m'
RED='\033[0;31m'
NC='\033[0m'

echo -e "${BLUE}=== QuneOs Setup Tool ===${NC}"

# 1. Handling the Submodule
if [ ! -f "linux/MAINTAINERS" ]; then
    echo -e "${BLUE}[1/4] Initializing and updating Linux submodule...${NC}"
    git submodule update --init --recursive --depth 1
else
    echo -e "${GREEN}[OK] Linux submodule already initialized.${NC}"
fi

# 2. Kernel Configuration and Compilation
cd linux
if [ ! -f ".config" ]; then
    echo -e "${BLUE}[2/4] Configuring kernel (defconfig)...${NC}"
    make defconfig
fi

if [ ! -f "arch/x86/boot/bzImage" ]; then
    echo -e "${BLUE}[2/4] Compiling kernel (this may take a while)...${NC}"
    make -j$(nproc)
else
    echo -e "${GREEN}[OK] Kernel is already compiled.${NC}"
fi
cd ..

# 3. Storage Creation
if [ ! -f "storage.img" ]; then
    echo -e "${BLUE}[3/4] Creating 100MB virtual disk image...${NC}"
    qemu-img create -f raw storage.img 100M
    mkfs.ext4 storage.img
else
    echo -e "${GREEN}[OK] storage.img already exists.${NC}"
fi

# 4. Finalizing
echo -e "${BLUE}[4/4] Setting permissions...${NC}"
chmod +x build.sh

echo -e "${GREEN}=== Setup complete! You can now run ./build.sh ===${NC}"
