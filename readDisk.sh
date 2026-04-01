#!/bin/bash
diskOut="disk"
DISK="disk.img"
SECTOR_SIZE=512
TABLE_SECTOR=12

mkdir -p "$diskOut"
rm -rf "$diskOut"/* "$diskOut"/.[!.]* "$diskOut"/..?*
for i in {0..15}
do
    OFFSET=$((TABLE_SECTOR * 512 + i * 41))
    TYPE=$(dd if=$DISK bs=1 skip=$((OFFSET + 32)) count=1 status=none | xxd -p)

    if [ "$TYPE" == "01" ]; then
        printf "%b" "$CONTENT" > "$DISKOUT/$NAME"
    elif [ "$TYPE" == "02" ]; then
        mkdir -p "$DISKOUT/$NAME"
    fi
done
