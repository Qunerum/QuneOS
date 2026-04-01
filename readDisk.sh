#!/bin/bash
diskOut="disk"
DISK="disk.img"
SECTOR_SIZE=512
ROOT_SECTOR=12

mkdir -p "$diskOut"
rm -rf "$diskOut"/*

read_directory() {
    local lba=$1
    local current_path=$2

    for i in {0..15}
    do
        local offset=$((lba * 512 + i * 41))

        local type=$(dd if=$DISK bs=1 skip=$((offset + 32)) count=1 status=none | xxd -p)

        [ "$type" == "00" ] && continue

        local name=$(dd if=$DISK bs=1 skip=$offset count=32 status=none | tr -d '\0' | tr -d ' ')
        [ -z "$name" ] && continue

        local lba_hex=$(dd if=$DISK bs=1 skip=$((offset + 33)) count=4 status=none | xxd -p)
        local b1=${lba_hex:0:2}; local b2=${lba_hex:2:2}; local b3=${lba_hex:4:2}; local b4=${lba_hex:6:2}
        local entry_lba=$((16#$b4$b3$b2$b1))

        if [ "$type" == "01" ]; then
            local content=$(dd if=$DISK bs=$SECTOR_SIZE skip=$entry_lba count=1 status=none | tr -d '\0')
            printf "%b" "$content" > "$current_path/$name"
        elif [ "$type" == "02" ]; then
            mkdir -p "$current_path/$name"
            read_directory "$entry_lba" "$current_path/$name"
        fi
    done
}

read_directory "$ROOT_SECTOR" "$diskOut"
