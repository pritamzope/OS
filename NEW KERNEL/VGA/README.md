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
	$ cd OS/NEW\ KERNEL/VGA
	$ make
```

**Open created ISO in QEMU:**
```
	$ qemu-system-x86_64 out/VGA.iso
```
or
```
	$ qemu-system-i386 out/VGA.iso
```
**Clean the code:**
```
	$ cd OS/NEW\ KERNEL/VGA
	$ make clean
```

<br/>
<br/>
<img src="https://raw.githubusercontent.com/pritamzope/OS/master/NEW%20KERNEL/VGA/kernel_vga.png"/>

