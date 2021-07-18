# Shell file to compile code

# assemble boot.s file
as --32 boot.s -o boot.o
as --32 load_gdt.s -o load_gdt.o

# compile kernel.c file
gcc -m32 -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

# compile gdt.c file
gcc -m32 -c gdt.c -o gdt.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

# linking all the object files to Global_Descriptor_Table_OS.bin
ld -m elf_i386 -T linker.ld kernel.o boot.o gdt.o load_gdt.o -o Global_Descriptor_Table_OS.bin -nostdlib

# check Global_Descriptor_Table_OS.bin file is x86 multiboot file or not
grub-file --is-x86-multiboot Global_Descriptor_Table_OS.bin

# building the iso file
mkdir -p isodir/boot/grub
cp Global_Descriptor_Table_OS.bin isodir/boot/Global_Descriptor_Table_OS.bin
cp grub.cfg isodir/boot/grub/grub.cfg
grub-mkrescue -o Global_Descriptor_Table_OS.iso isodir

# run it in qemu
qemu-system-x86_64 -cdrom Global_Descriptor_Table_OS.iso
