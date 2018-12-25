Requirements :-

GNU/Linux :-  Any distribution .
Assembler :-  GNU Assembler(gas)
GCC :-  GNU Compiler Collection a cross compiler. I'm using gcc 4, 5, 6, 7, 8 versions
Xorriso :-  A package that creates, loads, manipulates ISO 9660 filesystem images.(man xorriso)
grub-mkrescue :-  Make a GRUB rescue image, this package internally calls the xorriso functionality to build an iso image.
QEMU :-  Quick EMUlator to boot our kernel in virtual machine without rebooting the main system.


Usage :-

Run the script(run.sh)

	sh run.sh

References :-

https://www.codeproject.com/Articles/1225196/Create-Your-Own-Kernel-In-C

https://www.c-sharpcorner.com/article/create-your-own-kernel/
