Make sure you pass memory to qemu using -m option
At least minimum 4 MB memory is required


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

