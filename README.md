# OS
Simple Operating System programs starting from Bootloader in assembly to Kernel in C, and other programs like OS Calculator, Low Level Graphics etc.

Kernel contains C programs for Simple HelloWorld, Keyboard I/O etc... and little bit assembly programs for low level operations.

VGA contains C kernel and assembly program for seeting up Video Graphics Array(VGA) and drawing some basic shapes by ploting pixels.

Tic-Tac-Toe a simple tic-tac-toe DOS game with boxes, Pong-Game using graphics


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

&nbsp;1]&nbsp;&nbsp;Console<br/>
&nbsp;2]&nbsp;&nbsp;GDT<br/>
&nbsp;3]&nbsp;&nbsp;IDT<br/>
&nbsp;4]&nbsp;&nbsp;TSS<br>
&nbsp;5]&nbsp;&nbsp;Keyboard<br/>
&nbsp;6]&nbsp;&nbsp;Terminal<br/>
&nbsp;7]&nbsp;&nbsp;Timer<br/>
&nbsp;8]&nbsp;&nbsp;Mouse<br/>
&nbsp;9]&nbsp;&nbsp;FPU<br/>
10]&nbsp;&nbsp;Memory Info<br/>
11]&nbsp;&nbsp;Physical Memory Manager<br/>
12]&nbsp;&nbsp;KHeap<br/>
13]&nbsp;&nbsp;Paging<br/>
14]&nbsp;&nbsp;ATA read/write<br/>
15]&nbsp;&nbsp;BIOS 32<br/>
16]&nbsp;&nbsp;VGA<br/>
17]&nbsp;&nbsp;VESA VBE<br/>
18]&nbsp;&nbsp;Bitmap Text<br/>

