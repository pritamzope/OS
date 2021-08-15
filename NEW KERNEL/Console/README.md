## Requirements :-

(install following packages using apt-get)<br/>
1) GNU/Linux :-  Any distribution<br/>
2) make :- make utility<br/>
3) Assembler :-  NASM Assembler(nasm)<br/>
4) GCC :-  GNU Compiler Collection, C compiler<br/>
5) xorriso :-  A package that creates, loads, manipulates ISO 9660 filesystem images.(man xorriso)<br/>
6) grub-mkrescue :- utility to make ISO image<br/>
7) QEMU :-  Quick EMUlator to boot our kernel<br/>


## Usage :-<br/>

**Compile the source:**
```
	$ cd OS/NEW KERNEL/Console
	$ make
```

**Open created ISO in QEMU:**
```
	$ qemu-system-x86_64 out/Console.iso
```
or
```
	$ qemu-system-i386 out/Console.iso
```
**Clean the code:**
```
	$ cd OS/NEW KERNEL/Console
	$ make clean
```

### To Run kernel in VirtualBox :

1) Click on <b>New</b><br/>
2) Enter <b>Name</b>, <b>Type: Other</b>, <b>Version: Other/Unknown</b><br/>
3) Memory size : keep it as it is<br/>
4) Click next until <b>File location and size</b> dialog, select <b>64.00 MB</b> size<br/>
5) Click create<br/>
6) Goto <b>Settings -> System -> Acceleration</b> and disable Enable VT-x/AMD-V or any other graphics card support<br/>
7) Goto <b>Settings -> Storage</b> select ISO image of kernel.<br/>
8) Click OK and Start machine<br/>

