#assemble boot.s file
as --32 boot.s -o boot.o

#compile kernel.c file
gcc -m32 -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O1 -Wall -Wextra

gcc -m32 -c utils.c -o utils.o -std=gnu99 -ffreestanding -O1 -Wall -Wextra

gcc -m32 -c box.c -o box.o -std=gnu99 -ffreestanding -O1 -Wall -Wextra

gcc -m32 -c tic_tac_toe.c -o tic_tac_toe.o -std=gnu99 -ffreestanding -O1 -Wall -Wextra

#linking game files
ld -m elf_i386 -T linker.ld boot.o kernel.o utils.o box.o tic_tac_toe.o -o TicTacToe.bin -nostdlib

#check TicTacToe.bin file is x86 multiboot file or not
grub-file --is-x86-multiboot TicTacToe.bin

#building the iso file
mkdir -p isodir/boot/grub
cp TicTacToe.bin isodir/boot/TicTacToe.bin
cp grub.cfg isodir/boot/grub/grub.cfg
grub-mkrescue -o TicTacToe.iso isodir

#run it in qemu
qemu-system-x86_64 -cdrom TicTacToe.iso
