CROSS_COMPILE = aarch64-none-elf-
AS  = $(CROSS_COMPILE)as
CC  = $(CROSS_COMPILE)gcc
LD  = $(CROSS_COMPILE)ld
OBJCOPY = $(CROSS_COMPILE)objcopy
RM = rm -f

SRC = start.S main.c
OBJ = start.o main.o fs.o allocator.o safe_strings.o console.o prog_blob.o exec.o sample_blob.o
TARGET_ELF = kernel.elf
TARGET_IMG = kernel8.img
LINKER = link.ld

CFLAGS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding
LDFLAGS = -T $(LINKER)

all: $(TARGET_IMG)

$(TARGET_IMG): $(TARGET_ELF)
	$(OBJCOPY) $< -O binary $@

$(TARGET_ELF): $(OBJ)
	$(LD) $(LDFLAGS) $(OBJ) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.S
	$(AS) -c $< -o $@

clean:
	$(RM) *.o *.elf *.img

.PHONY: all clean

# Build object from assembly
prog.o: prog.S
	aarch64-none-elf-as -o prog.o prog.S

# Link to ELF file
prog.elf: prog.o
	aarch64-none-elf-ld -T prog.ld -o prog.elf prog.o

# Convert ELF to raw binary
prog.bin: prog.elf
	aarch64-none-elf-objcopy -O binary prog.elf prog.bin

# Convert raw binary to blob object
prog_blob.o: prog.bin
	aarch64-none-elf-objcopy -I binary -O elf64-littleaarch64 -B aarch64 prog.bin prog_blob.o

sample_blob.o: sample.bin
	aarch64-none-elf-objcopy -I binary -O elf64-littleaarch64 -B aarch64 sample.bin sample_blob.o
