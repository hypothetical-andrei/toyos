# llc -march=aarch64 sample.ll -o sample.s
# aarch64-linux-gnu-gcc -nostdlib -static -Ttext=0x0 sample.s -o sample.elf
# aarch64-linux-gnu-objcopy -O binary sample.elf sample.bin

llc -march=aarch64 -filetype=obj sample.ll -o sample.o
aarch64-none-elf-ld -Ttext=0x0 -e main -o sample.elf sample.o
aarch64-none-elf-objcopy -O binary sample.elf sample.bin
aarch64-none-elf-objcopy -I binary -O elf64-littleaarch64 -B aarch64 sample.bin sample_blob.o
