[bits 16]
[org 0x7c00]

kernel16_start:

    xor ax, ax
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov sp, 0xfff

    cli

    lgdt[GDT_DESCRIPTOR]

    mov eax, cr0
    or eax, 1
    mov cr0, eax

    jmp CODE_SEGMENT:kernel32_start


[bits 32]

kernel32_start:

    mov ax, DATA_SEGMENT
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    mov ebp, 0x0ffffff
    mov esp, ebp

    mov esi, hello_world
    call print_string

.loopk:
    jmp .loopk


print_string:
    pusha
    mov edx, VIDEO_MEMORY
.loop:
    mov al, [esi]
    cmp al, 0
    je .exit
    mov ah, 0x0f
    mov [edx], ax
    add esi, 1
    add edx, 2
    jmp .loop
.exit:
    popa
    ret


; Global Descriptor Table definition

GDT_START:

; GDT null segment (2 dwords)
GDT_NULL:
    dd 0x0
    dd 0x0

; GDT Code segment (2 dwords)
GDT_CODE:
    dw 0xffff  ; segment limit first 0-15 bits
    dw 0x0  ; base first 0-15 bits
    db 0x0  ; base 16-23 bits
    db 0x9a  ; access byte
    db 11001111b  ; high 4 bits (flags) low 4 bits (limit 4 last bits)(limit is 20 bit wide)
    db 0x0  ; base 24-31 bits

; GDT Data Segment (2 dwords)
GDT_DATA:
    dw 0xffff  ; segment limit first 0-15 bits
    dw 0x0  ; base first 0-15 bits
    db 0x0  ; base 16-23 bits
    db 0x92  ; access byte
    db 11001111b  ; high 4 bits (flags) low 4 bits (limit 4 last bits)(limit is 20 bit wide)
    db 0x0  ; base 24-31 bits

GDT_END:

GDT_DESCRIPTOR:
    dw GDT_END - GDT_START
    dd GDT_START

CODE_SEGMENT equ GDT_CODE + 4
DATA_SEGMENT equ GDT_DATA + 4


hello_world db 'HelloWorld', 0

VIDEO_MEMORY equ 0xB8000


times (510 - ($ - $$)) db 0x00
dw 0AA55h


