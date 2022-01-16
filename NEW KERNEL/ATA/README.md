## Requirements :-

Install following packages using apt-get<br/>
1) GNU/Linux :-  Any distribution<br/>
2) make :- make utility<br/>
3) Assembler :-  NASM Assembler(nasm)<br/>
4) GCC :-  GNU Compiler Collection, C compiler<br/>
5) xorriso :-  A package that creates, loads, manipulates ISO 9660 filesystem images.(man xorriso)<br/>
6) grub-mkrescue :- utility to make ISO image<br/>
7) Virtual Box :-  Virtual box to check our drive read/write<br/>
8) GHex :- Graphical hex editor

## Output :-<br/>
<img src="https://raw.githubusercontent.com/pritamzope/OS/master/NEW%20KERNEL/ATA/screenshots/ata_output.png"/>
<br/>

## Usage :-<br/>

**Compile the source:**
```
	$ cd OS/NEW\ KERNEL/ATA
	$ make
```

**Clean the code:**
```
	$ cd OS/NEW\ KERNEL/ATA
	$ make clean
```


## Running in Virtual Box :-<br/>

**Install Virtual Box:**
```
	$ sudo apt-get install virtualbox
```
<br/>

1. Start Virtual Box & click on New
<br/>
<img src="https://raw.githubusercontent.com/pritamzope/OS/master/NEW%20KERNEL/ATA/screenshots/ata_1.png"/>
<br/>

2. Enter machine name with <b>Type = Other</b> and <b>Version = Other/Unknown</b>
<br/>
<img src="https://raw.githubusercontent.com/pritamzope/OS/master/NEW%20KERNEL/ATA/screenshots/ata_2.png"/>
<br/>

3. Select minimum memory required, for our kernel 64MB is more than enough
<br/>
<img src="https://raw.githubusercontent.com/pritamzope/OS/master/NEW%20KERNEL/ATA/screenshots/ata_3.png"/>
<br/>

4. Select <b>Create a virtual harddisk now</b>
<br/>
<img src="https://raw.githubusercontent.com/pritamzope/OS/master/NEW%20KERNEL/ATA/screenshots/ata_4.png"/>
<br/>

5. Select <b>VHD (Virtual Hard Disk)</b> disk type
   You can select any other type too but VHD are empty(no extra data in it)
   Easier to check where read/write operations are being performed correctly
<br/>
<img src="https://raw.githubusercontent.com/pritamzope/OS/master/NEW%20KERNEL/ATA/screenshots/ata_5.png"/>
<br/>

6. Select <b>Fixed size</b>
   Because we need fixed size disk to test our kernel read/write operations from outside(using hexedit/GHex app)
<br/>
<img src="https://raw.githubusercontent.com/pritamzope/OS/master/NEW%20KERNEL/ATA/screenshots/ata_6.png"/>
<br/>

7. Select atleast 512MB size of the disk
<br/>
<img src="https://raw.githubusercontent.com/pritamzope/OS/master/NEW%20KERNEL/ATA/screenshots/ata_7.png"/>
<br/>

8. Select the build <b>ATA.iso</b> image file to boot our kernel
<br/>
<img src="https://raw.githubusercontent.com/pritamzope/OS/master/NEW%20KERNEL/ATA/screenshots/ata_8.png"/>
<br/>


## Checking VHD contents in GHex :-<br/>

Install GHex by following command:
```
	$ sudo apt-get install ghex
```

Go to Virtual Box VM's and Open ATA.vhd file
```
	$ ghex ATA.vhd
```
<br/>
<img src="https://raw.githubusercontent.com/pritamzope/OS/master/NEW%20KERNEL/ATA/screenshots/ata_output_check.png"/>
<br/>
