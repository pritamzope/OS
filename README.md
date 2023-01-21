# OS
Simple Operating System programs starting from Bootloader in assembly to Kernel in C, and other programs like OS Calculator, Low Level Graphics etc.

Kernel contains C programs for Simple HelloWorld, Keyboard I/O etc... and little bit assembly programs for low level operations.

VGA contains C kernel and assembly program for seeting up Video Graphics Array(VGA) and drawing some basic shapes by ploting pixels.

Tic-Tac-Toe a simple tic-tac-toe DOS game with boxes, Pong-Game using graphics


# Requirements :-

(install following packages using apt-get)<br/>
1) GNU/Linux :-  Any distribution<br/>
2) make :- make utility<br/>
3) Nasm :-  NASM Assembler(nasm)<br/>
4) GCC :-  GNU Compiler Collection, C compiler<br/>
5) xorriso :-  A package that creates, loads, manipulates ISO 9660 filesystem images.(man xorriso)<br/>
6) grub-pc-bin: GRUB binaries and modules<br/>
7) mtools: utilities to access DOS disks in Unix<br/>
8) grub-mkrescue :- utility to make ISO image<br/>
9) QEMU :-  Quick EMUlator to boot our kernel<br/>
10) VirtualBox :- A full graphical virtual environment


```
$ sudo apt-get install make nasm gcc grub-pc-bin mtools xorriso qemu qemu-system virtualbox
```


# NEW KERNEL

A new modified source code has been added to "NEW KERNEL" directory.
Compilation scripts are replaced with Makefile.
Assembly code is replaced with NASM assembly rathen than GNU AS.

## Order

&nbsp;&nbsp;1]&nbsp;&nbsp;&nbsp;Console<br/>
&nbsp;&nbsp;2]&nbsp;&nbsp;&nbsp;GDT<br/>
&nbsp;&nbsp;3]&nbsp;&nbsp;&nbsp;IDT<br/>
&nbsp;&nbsp;4]&nbsp;&nbsp;&nbsp;TSS<br>
&nbsp;&nbsp;5]&nbsp;&nbsp;&nbsp;Keyboard<br/>
&nbsp;&nbsp;6]&nbsp;&nbsp;&nbsp;Terminal<br/>
&nbsp;&nbsp;7]&nbsp;&nbsp;&nbsp;Timer<br/>
&nbsp;&nbsp;8]&nbsp;&nbsp;&nbsp;Mouse<br/>
&nbsp;&nbsp;9]&nbsp;&nbsp;&nbsp;FPU<br/>
10]&nbsp;&nbsp;&nbsp;Memory Info<br/>
11]&nbsp;&nbsp;&nbsp;Physical Memory Manager<br/>
12]&nbsp;&nbsp;&nbsp;KHeap<br/>
13]&nbsp;&nbsp;&nbsp;Paging<br/>
14]&nbsp;&nbsp;&nbsp;ATA read/write<br/>
15]&nbsp;&nbsp;&nbsp;BIOS 32<br/>
16]&nbsp;&nbsp;&nbsp;VGA<br/>
17]&nbsp;&nbsp;&nbsp;VESA VBE<br/>
18]&nbsp;&nbsp;&nbsp;Bitmap Text<br/>

