Requirements :-

GNU/Linux :-  Any distribution<br/>
Assembler :-  GNU Assembler(gas)<br/>
GCC :-  GNU Compiler Collection, C compiler. I'm using gcc 4, 5, 6, 7, 8 versions<br/>
Xorriso :-  A package that creates, loads, manipulates ISO 9660 filesystem images.(man xorriso)<br/>
grub-mkrescue :-  Make a GRUB rescue image, this package internally calls the xorriso functionality to build an iso image.<br/>
QEMU :-  Quick EMUlator to boot our kernel in virtual machine without rebooting the main system.<br/>


Usage :-<br/>

Run the script(run.sh)

	sh run.sh
	
or

	chmod +x run.sh
	./run.sh
	

To Run kernel in VirtualBox :

1) Click on <b>New</b><br/>
2) Enter <b>Name</b>, <b>Type: Other</b>, <b>Version: Other/Unknown</b><br/>
3) Memory size : keep it as it is<br/>
4) Click next until <b>File location and size</b> dialog, select <b>64.00 MB</b> size<br/>
5) Click create<br/>
6) Goto <b>Settings -> System -> Acceleration</b> and disable Enable VT-x/AMD-V or any other graphics card support<br/>
7) Goto <b>Settings -> Storage</b> select ISO image of kernel.<br/>
8) Click OK and Start machine<br/>




References :-

https://www.codeproject.com/Articles/1225196/Create-Your-Own-Kernel-In-C

https://www.c-sharpcorner.com/article/create-your-own-kernel/
