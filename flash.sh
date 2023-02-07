pio ci --lib="./lib/*" -b esp32dev -e ESP32 --project-option="lib_tdf_mode=chain+" --keep-build-dir --project-conf=./platformio.ini --build-dir="$HOME" ./dustboy_vm
TARGET="."
BOARD_ENV="ESP32"

cp -Rv .pio/build/$BOARD_ENV/firmware.bin "$TARGET"
cp -Rv .pio/build/$BOARD_ENV/partitions.bin "$TARGET"
cp -Rv .pio/build/$BOARD_ENV/bootloader.bin "$TARGET"

cp -Rv $HOME/.platformio/packages/framework-arduinoespressif32/tools/partitions/boot_app0.bin "$TARGET"
cp -Rv .pio/build/$BOARD_ENV/firmware.bin" $TARGET"

"/home/ubuntu/.platformio/penv/bin/python" "/home/ubuntu/.platformio/packages/tool-esptoolpy/esptool.py" --chip esp32   --baud 460800   --before default_reset   --after hard_reset   write_flash   -z   --flash_mode dio   --flash_freq 40m   --flash_size 4MB   0x1000 bootloader.bin 0x8000 partitions.bin 0xe000 boot_app0.bin 0x10000 firmware.bin
