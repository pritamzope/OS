# set magic number to 0x1BADB002 to identified by bootloader 
.set MAGIC,    0x1BADB002

# set flags to 0
.set FLAGS,    0

# set the checksum
.set CHECKSUM, -(MAGIC + FLAGS)

# set multiboot enabled
.section .multiboot

# define type to long for each data defined as above
.long MAGIC
.long FLAGS
.long CHECKSUM


# set the stack bottom 
stackBottom:

# define the maximum size of stack to 512 bytes
.skip 1024


# set the stack top which grows from higher to lower
stackTop:

.section .text
.global _start
.global load_gdt
.type _start, @function

load_gdt:
	mov 4(%esp), %eax
	lgdt (%eax)
	
	mov $0x10, %eax
	mov %eax, %ds
	mov %eax, %es
	mov %eax, %fs
	mov %eax, %gs
	mov %eax, %ss
	jmp $0x8, $.long_jump
.long_jump:
	ret


_start:

  # assign current stack pointer location to stackTop
	mov $stackTop, %esp

  # call the kernel main source
	call kernel_entry

	cli


# put system in infinite loop
hltLoop:

	hlt
	jmp hltLoop

.size _start, . - _start


