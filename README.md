# OS
Simple Operating System programs starting from Bootloader in assembly to Kernel in C, and other programs like OS Calculator, Low Level Graphics etc.

Kernel contains C programs for Simple HelloWorld, Keyboard I/O etc... and little bit assembly programs for low level operations.

VGA contains C kernel and assembly program for seeting up Video Graphics Array(VGA) and drawing some basic shapes by ploting pixels.

Tic-Tac-Toe a simple tic-tac-toe DOS game with boxes, Pong-Game using graphics


First read this page before you start: https://wiki.osdev.org/Beginner_Mistakes


# Requirements :-

(install following packages using apt-get)<br/>
1) GNU/Linux :-  Any distribution<br/>
2) make :- make utility<br/>
3) Assembler :-  NASM Assembler(nasm)<br/>
4) GCC :-  GNU Compiler Collection, C compiler<br/>
5) xorriso :-  A package that creates, loads, manipulates ISO 9660 filesystem images.(man xorriso)<br/>
6) grub-mkrescue :- utility to make ISO image<br/>
7) QEMU :-  Quick EMUlator to boot our kernel<br/>
8) VirtualBox :- A full graphical virtual environment


```
$ sudo apt-get install make nasm gcc xorriso qemu qemu-system-x86 qemu-system-i386 virtualbox
```


# NEW KERNEL

A new modified source code has been added to "NEW KERNEL" directory.
Compilation scripts are replaced with Makefile.
Assembly code is replaced with NASM assembly rathen than GNU AS.

## Order

1] Console<br/>
2] GDT<br/>
3] IDT<br/>
4] Keyboard<br/>
5] Terminal<br/>
6] Timer<br/>
7] Mouse<br/>
8] FPU<br/>
9] Memory Info<br/>
10] Physical Memory Manager<br/>
11] ATA read/write<br/>
12] BIOS 32<br/>

