ENTRY(_start)

SECTIONS
{
    /* text section */
    .text 0x0100000 : {
        . = ALIGN(4);
        /* set multiboot header */
        LONG(0x1BADB002)
        LONG(0x00000003)
        LONG(0xE4524FFB)
        code = .; _code = .; __code = .;
        *(.text)
        *(.rodata)
        . = ALIGN(4096);
    }

    /* initialized data section */
    .data : {
        data = .; _data = .; __data = .;
        *(.data)
        . = ALIGN(4096);
    }

    /* constant data section */
    .rodata : {
        *(.rodata)
    }

    /* un-initialized data section */
    .bss : {
        bss = .; _bss = .; __bss = .;
        *(.bss)
        . = ALIGN(4096);
    }

    end = .; _end = .; __end = .;
    kernel_end = .;
}