#!/bin/bash
# PeepHole Build Script
# Needs: nasm, i686-elf-gcc, i686-elf-ld, i686-elf-objcopy
# If you have system gcc with multilib, you can use gcc -m32 instead

set -e
mkdir -p build
echo "[1/5] Building PIGGY.COM loader..."
nasm -f bin src/boot/phase2_loader.asm -o build/PIGGY.COM

echo "[2/5] Assembling kernel entry..."
nasm -f elf32 src/kernel_entry.asm -o build/kernel_entry.o


# Try i686-elf-gcc first, fallback to gcc -m32 if you have cross compiler not installed
if command -v i686-elf-gcc >/dev/null 2>&1; then
  CC="i686-elf-gcc"
  LD="i686-elf-ld"
  OBJCOPY="i686-elf-objcopy"
else
  echo "i686-elf-gcc not found, using gcc -m32 (requires multilib)"
  CC="gcc"
  LD="ld"
  OBJCOPY="objcopy"
fi
echo "[3/5] Compiling lib ..."
$CC -m32 -ffreestanding -fno-pie -fno-stack-protector -nostdlib -fno-builtin -I./includes -c lib/vga.c -o build/vga.o
$CC -m32 -ffreestanding -fno-pie -fno-stack-protector -nostdlib -fno-builtin -I./includes -c lib/string.c -o build/string.o

echo "[4/5] Compiling kernel.c..."
$CC -m32 -ffreestanding -fno-pie -fno-stack-protector -nostdlib -fno-builtin -I./includes -c src/kernel.c -o build/kernel.o
$CC -m32 -ffreestanding -fno-pie -fno-stack-protector -nostdlib -fno-builtin -I./includes -c src/pmm.c -o build/pmm.o
$CC -m32 -ffreestanding -fno-pie -fno-stack-protector -nostdlib -fno-builtin -I./includes -c src/paging.c -o build/paging.o
$CC -m32 -ffreestanding -fno-pie -fno-stack-protector -nostdlib -fno-builtin -I./includes -c src/idt.c -o build/idt.o
$CC -m32 -ffreestanding -fno-pie -fno-stack-protector -nostdlib -fno-builtin -I./includes -c src/irq.c -o build/irq.o
$CC -m32 -ffreestanding -fno-pie -fno-stack-protector -nostdlib -fno-builtin -I./includes -c src/vmm.c -o build/vmm.o
nasm -f elf32 src/isr.asm -o build/isr.o

echo "[5/5] Linking KERNEL.BIN at 0x100000..."
$LD -T linker.ld -m elf_i386 -nostdlib build/kernel_entry.o build/kernel.o build/vga.o build/string.o build/pmm.o build/paging.o \
       build/irq.o build/idt.o build/isr.o build/vmm.o -o build/KERNEL.ELF
$OBJCOPY -O binary build/KERNEL.ELF build/KERNEL.BIN

echo ""
echo "BUILD OK!"
ls -lh build/PIGGY.COM build/KERNEL.BIN build/KERNEL.ELF
echo ""
echo "To run in DOSBox:"
echo "  dosbox -c 'mount D $(pwd)' -c 'D:' -c 'PIGGY.COM'"
echo "Or manually:"
echo "  mount D $(pwd)"
echo "  D:"
echo "  PIGGY.COM"
