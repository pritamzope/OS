ENTRY(_start)

SECTIONS
{
    __kernel_section_start = .;
    /* text section */
    .text 0x0100000 : {
        __kernel_text_section_start = .;
        . = ALIGN(4);
        /* set multiboot header */
        LONG(0x1BADB002)
        LONG(0x00000003)
        LONG(0xE4524FFB)
        code = .; _code = .; __code = .;
        *(.text)
        *(.rodata)
        . = ALIGN(4096);
        __kernel_text_section_end = .;
    }

    /* initialized data section */
    .data : {
        __kernel_data_section_start = .;
        data = .; _data = .; __data = .;
        *(.data)
        . = ALIGN(4096);
        __kernel_data_section_end = .;
    }

    /* constant data section */
    .rodata : {
        __kernel_rodata_section_start = .;
        *(.rodata)
        __kernel_rodata_section_end = .;
    }

    /* un-initialized data section */
    .bss : {
        __kernel_bss_section_start = .;
        bss = .; _bss = .; __bss = .;
        *(.bss)
        . = ALIGN(4096);
        __kernel_bss_section_end = .;
    }

    end = .; _end = .; __end = .;
    __kernel_section_end = .;
}

