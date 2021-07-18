# Shell file to compile code

# assemble boot.s file
as --32 boot.s -o boot.o

# compile kernel.c file
gcc -m32 -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

# linking all the object files to CPUInfo_OS.bin
ld -m elf_i386 -T linker.ld kernel.o boot.o -o CPUInfo_OS.bin -nostdlib

# check CPUInfo_OS.bin file is x86 multiboot file or not
grub-file --is-x86-multiboot CPUInfo_OS.bin

# building the iso file
mkdir -p isodir/boot/grub
cp CPUInfo_OS.bin isodir/boot/CPUInfo_OS.bin
cp grub.cfg isodir/boot/grub/grub.cfg
grub-mkrescue -o CPUInfo_OS.iso isodir

# run it in qemu
qemu-system-x86_64 -cdrom CPUInfo_OS.iso
