NOTE: Bitmap memory allocator works only on small memory size
      If want to make it work for large memory, make sure to increse BLOCK size(2^n)
      Here i'm using only one memory manager using bitmap allocation which is extremely slow.
      we will add other types such as Linked allocation or Buddy system...
      
      Make sure you pass memory to qemu using -m option

## Usage :-<br/>

**Compile the source:**
```
	$ cd OS/NEW\ KERNEL/Physical\ Memory\ Manager
	$ make
```

**Open created ISO in QEMU:**
```
	$ qemu-system-x86_64 -m 64 out/Physical_MM.iso
```
or
```
	$ qemu-system-i386 out/Physical_MM.iso
```
**Clean the code:**
```
	$ cd OS/NEW\ KERNEL/Physical\ Memory\ Manager
	$ make clean
```

<br/>
<br/>
<img src="https://raw.githubusercontent.com/pritamzope/OS/master/NEW%20KERNEL/Physical%20Memory%20Manager/kernel_physical_mm.png"/>

