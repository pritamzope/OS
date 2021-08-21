section .text
    global load_gdt

load_gdt:
    mov eax, [esp + 4]  ; get gdt pointer
    lgdt [eax]          ; load gdt

    mov ax, 0x10    ; kernel data segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    cli             ; clear interrupts
    mov	eax, cr0    ; set bit 0 in cr0 to enter protected mode
    or eax, 1
    mov	cr0, eax

    jmp 0x08:far_jump   ; jump to far with code data segment
far_jump:
    ret

