#assemble boot.s file
as boot.s -o boot.o

#compile kernel.c file
gcc -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

#linking the kernel with kernel.o and boot.o files
gcc -T linker.ld -o MyOS.bin -ffreestanding -O2 -nostdlib kernel.o boot.o -lgcc

#check MyOS.bin file is x86 multiboot file or not
grub-file --is-x86-multiboot MyOS.bin

#building the iso file
mkdir -p isodir/boot/grub
cp MyOS.bin isodir/boot/MyOS.bin
cp grub.cfg isodir/boot/grub/grub.cfg
grub-mkrescue -o MyOS.iso isodir

#run it in qemu
qemu-system-x86_64 -cdrom MyOS.iso
