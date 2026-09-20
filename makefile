# === Toolchain ===
CC = gcc
AS = as
LD = ld
NASM = nasm
QEMU = qemu-system-i386

# === Flags ===
CFLAGS = -ffreestanding -m32 -nostdlib -nostartfiles -nodefaultlibs \
         -Wall -Wextra -O2 -I src -fno-stack-protector -fno-pic

ASFLAGS = -f elf32
LDFLAGS = -m elf_i386 -T linker.ld -nostdlib

# === Sources ===
C_SOURCES = src/kernel.c \
            src/pmm.c \
            src/paging.c \
            src/vmm.c \
            src/idt.c \
            src/isr.c \
            src/gdt.c \
            src/io.c

# Add kheap.c after Layer A test passes
# C_SOURCES += src/kheap.c

ASM_SOURCES = src/boot.s

C_OBJS = $(C_SOURCES:.c=.o)
ASM_OBJS = $(ASM_SOURCES:.s=.o)
OBJS = $(ASM_OBJS) $(C_OBJS)

KERNEL = kernel.bin
ISO = os.iso

all: $(KERNEL)

# Link
$(KERNEL): $(OBJS) linker.ld
	$(LD) $(LDFLAGS) -o $@ $(OBJS)
	@echo "Built $(KERNEL) - size $$(stat -c%s $(KERNEL)) bytes"

# C files
src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# ASM files - multiboot boot.s
src/%.o: src/%.s
	$(NASM) $(ASFLAGS) $< -o $@

# Run
run: $(KERNEL)
	$(QEMU) -kernel $(KERNEL) -m 128M -serial stdio

run-iso: $(ISO)
	$(QEMU) -cdrom $(ISO) -m 128M -serial stdio

$(ISO): $(KERNEL)
	mkdir -p isodir/boot/grub
	cp $(KERNEL) isodir/boot/kernel.bin
	echo 'set timeout=0\nset default=0\nmenuentry "myos" { multiboot /boot/kernel.bin }' > isodir/boot/grub/grub.cfg
	grub-mkrescue -o $(ISO) isodir

clean:
	rm -f $(OBJS) $(KERNEL) $(ISO)
	rm -rf isodir

# Debug - show sections
symbols: $(KERNEL)
	nm $(KERNEL) | sort

# Help
help:
	@echo "make       - build kernel.bin"
	@echo "make run   - qemu -kernel"
	@echo "make clean - clean"
