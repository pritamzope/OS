#assemble boot.s file
as --32 boot.s -o boot.o

#compile kernel.c file
gcc -m32 -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O1 -Wall -Wextra

#compile vga.c file
gcc -m32 -c vga.c -o vga.o -std=gnu99 -ffreestanding -O1 -Wall -Wextra

#compile bitmap.c file
gcc -m32 -c bitmap.c -o bitmap.o -std=gnu99 -ffreestanding -O1 -Wall -Wextra

#compile utils.c file
gcc -m32 -c utils.c -o utils.o -std=gnu99 -ffreestanding -O1 -Wall -Wextra

#compile pong.c file
gcc -m32 -c pong.c -o pong.o -std=gnu99 -ffreestanding -O1 -Wall -Wextra

#linking the kernel with object files
ld -m elf_i386 -T linker.ld kernel.o vga.o boot.o bitmap.o utils.o pong.o -o Pong_Game.bin -nostdlib

#check Pong_Game.bin file is x86 multiboot file or not
grub-file --is-x86-multiboot Pong_Game.bin

#building the iso file
mkdir -p isodir/boot/grub
cp Pong_Game.bin isodir/boot/Pong_Game.bin
cp grub.cfg isodir/boot/grub/grub.cfg
grub-mkrescue -o Pong_Game.iso isodir

#run it in qemu
qemu-system-x86_64 -cdrom Pong_Game.iso
