# output from args
output=$1
if [ -z "$output" ]; then
	echo "Error: No output file specified"
	exit 1
fi
board=esp32dev
# if no dir fw, create it
if [ ! -d "fw" ]; then
	mkdir fw
fi
# alias esptool.py=esptool
alias esptool=esptool.py
esptool --chip esp32 merge_bin \
	-o fw/$output \
	--flash_mode dio \
	--flash_freq 40m \
	--flash_size 4MB \
	0x1000 .pio/build/$board/bootloader.bin \
	0x8000 .pio/build/$board/partitions.bin \
	0x10000 .pio/build/$board/firmware.bin
 
