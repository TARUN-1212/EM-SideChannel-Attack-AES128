# Makefile for building bare-metal AES128 firmware for Raspberry Pi 4
# Builds a bootable kernel8.img using GCC ARM toolchain
# Author: Sai Tarun Baratam (DRDO Internship Project)

# ========== Toolchain Configuration ==========

GCCPATH   = /mnt/d/Tarun/gcc-arm-10.3-2021.07-x86_64-aarch64-none-elf/bin
GCCFLAGS  = -Wall -O2 -ffreestanding -nostdinc -nostdlib -nostartfiles \
            -I/mnt/d/Tarun/gcc-arm-10.3-2021.07-x86_64-aarch64-none-elf/aarch64-none-elf/include \
            -I/mnt/d/Tarun/gcc-arm-10.3-2021.07-x86_64-aarch64-none-elf/lib/gcc/aarch64-none-elf/10.3.1/include

# ========== Source and Object Files ==========

CFILES = $(wildcard *.c)
OFILES = $(CFILES:.c=.o)

# ========== Build Targets ==========

# Default target
all: clean kernel8.img

# Compile startup assembly file
boot.o: boot.S
	$(GCCPATH)/aarch64-none-elf-gcc $(GCCFLAGS) -c boot.S -o boot.o

# Compile all .c files to .o files
%.o: %.c
	$(GCCPATH)/aarch64-none-elf-gcc $(GCCFLAGS) -c $< -o $@

# Link all object files and generate kernel image
kernel8.img: boot.o $(OFILES)
	$(GCCPATH)/aarch64-none-elf-ld -nostdlib boot.o $(OFILES) -T link.ld -o kernel8.elf
	$(GCCPATH)/aarch64-none-elf-objcopy -O binary kernel8.elf kernel8.img

# Clean build artifacts
clean:
	/bin/rm -f kernel8.elf *.o *.img > /dev/null 2>&1 || true
