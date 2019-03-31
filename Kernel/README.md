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
	
or

	chmod +x run.sh
	./run.sh
	

To Run kernel in VirtualBox :

Click on <b>New</b>
Enter <b>Name</b>, <b>Type: Other</b>, <b>Version: Other/Unknown</b>
Memory size : keep it as it is
Click next until <b>File location and size</b> dialog, select <b>64.00 MB</b> size
Click create
Goto <b>Settings -> System -> Acceleration</b> and disable Enable VT-x/AMD-V or any other graphics card support
Goto <b>Settings -> Storage</b> select ISO image of kernel.
Click OK and Start machine




References :-

https://www.codeproject.com/Articles/1225196/Create-Your-Own-Kernel-In-C

https://www.c-sharpcorner.com/article/create-your-own-kernel/
