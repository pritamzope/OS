# Shell file to compile code

# assemble boot.s file
as --32 boot.s -o boot.o

# compile kernel.c file
gcc -m32 -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O1 -Wall -Wextra

gcc -m32 -c utils.c -o utils.o -std=gnu99 -ffreestanding -O1 -Wall -Wextra

# linking all the object files to GUI_DosBox_OS.bin
ld -m elf_i386 -T linker.ld kernel.o utils.o boot.o -o GUI_DosBox_OS.bin -nostdlib

# check GUI_DosBox_OS.bin file is x86 multiboot file or not
grub-file --is-x86-multiboot GUI_DosBox_OS.bin

# building the iso file
mkdir -p isodir/boot/grub
cp GUI_DosBox_OS.bin isodir/boot/GUI_DosBox_OS.bin
cp grub.cfg isodir/boot/grub/grub.cfg
grub-mkrescue -o GUI_DosBox_OS.iso isodir

# run it in qemu
qemu-system-x86_64 -cdrom GUI_DosBox_OS.iso
