[bits 32]

%define REBASE_ADDRESS(x)  (0x7c00 + ((x) - BIOS32_START))

section .text
    global BIOS32_START ; start memory address
    global BIOS32_END  ; end memory address

    global bios32_gdt_ptr  ; GDT pointer
    global bios32_gdt_entries ; GDT array entries
    global bios32_idt_ptr ; IDT pointer
    global bios32_in_reg16_ptr ; IN REGISTERS16 
    global bios32_out_reg16_ptr ; OUT REGISTERS16
    global bios32_int_number_ptr ; bios interrupt number to be called

ENABLE_PAGING equ 0x80000001
DISABLE_PAGING equ 0x7FFFFFFF

; 32 bit protected mode
BIOS32_START:use32
    pushf
    pusha

    ; Save the original IDTR and GDTR onto the stack
    sub esp, 32
    sidt [esp]
    sgdt [esp+16]

    ; Save current protected mode ESP
    mov [REBASE_ADDRESS(prot_esp)], esp

    ; Jumping to 16-bit protected mode, disable interrupts
    cli

    ; Turn off paging
    mov eax, cr0
    and eax, DISABLE_PAGING
    mov cr0, eax

    ; Flush the TLB
    mov eax, cr3
    mov cr3, eax


    ; load new empty GDT
    lgdt [REBASE_ADDRESS(bios32_gdt_ptr)]
    ; load new empty IDT
    lidt [REBASE_ADDRESS(bios32_idt_ptr)]
    ; jump to 16 bit protected mode
    jmp 0x30:REBASE_ADDRESS(__protected_mode_16)

; 16 bit protected mode
__protected_mode_16:use16
    ; jumping to 16 bit real mode
    mov ax, 0x38
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    ; turn off protected mode
    ; set bit 0 to 0
    mov eax, cr0
    and al,  ~0x01
    mov cr0, eax
    jmp 0x0:REBASE_ADDRESS(__real_mode_16)

; 16 bit real mode
__real_mode_16:use16
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov sp, 0x8c00
    ; bios interrupts cannot be enabled because IRQs have been remapped, external interrupts will cause page faults/bad things
    ; sti
    ; save current context, all general, segment registers, flags
    pusha
    mov cx, ss
    push cx
    mov cx, gs
    push cx
    mov cx, fs
    push cx
    mov cx, es
    push cx
    mov cx, ds
    push cx
    pushf
    ; get current stack pointer & save it to current_esp
    mov eax, esp
    mov edi, REBASE_ADDRESS(current_esp)
    stosd
    ; load our custom registers context
    mov sp, REBASE_ADDRESS(bios32_in_reg16_ptr)
    ; only use some general register from the given context
    popa
    ; set a new stack for bios interrupt
    mov sp, 0x9c00
    ; call immediate interrupt opcode to execute context
    db 0xCD

bios32_int_number_ptr: ; will be bios interrupt number passed
    ; put the actual interrupt number here
    db 0x00
    ; get our output context here
    mov esp, REBASE_ADDRESS(bios32_out_reg16_ptr)
    add sp, 28 ; restore stack used for calling our context
    ; save current context, all general, segment registers, flags
    pushf
    mov cx, ss
    push cx
    mov cx, gs
    push cx
    mov cx, fs
    push cx
    mov cx, es
    push cx
    mov cx, ds
    push cx
    pusha
    ; restore the current_esp to continue
    mov esi, REBASE_ADDRESS(current_esp)
    lodsd
    mov esp, eax
    ; restore all current context, all general, segment registers, flags
    popf
    pop cx
    mov ds, cx
    pop cx
    mov es, cx
    pop cx
    mov fs, cx
    pop cx
    mov gs, cx
    pop cx
    mov ss, cx
    popa

    ; jumping to 32 bit protected mode
    ; set bit 0 in cr0 to 1
    cli
    mov eax, cr0
    inc eax
    mov cr0, eax
    jmp 0x08:REBASE_ADDRESS(__protected_mode_32)

; 32 bit protected mode
__protected_mode_32:use32
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; retore esp
    mov esp, [REBASE_ADDRESS(prot_esp)]

    ; Reload the original IDTr and GDTr from the stack
    lidt [esp]
    lgdt [esp + 16]
    add esp, 32

    ; Restore all registers
    popa

    ; Restore flags including IF state
    popf
    ret


align 4

bios32_gdt_entries:
    ; 8 gdt entries
    TIMES 8 dq 0
bios32_gdt_ptr:
    dd 0x00000000
    dd 0x00000000
bios32_idt_ptr:
    dd 0x00000000
    dd 0x00000000
bios32_in_reg16_ptr:
    resw 14
bios32_out_reg16_ptr:
    dd 0xaaaaaaaa
    dd 0xaaaaaaaa
    dd 0xaaaaaaaa
    dd 0xaaaaaaaa
    dd 0xaaaaaaaa
    dd 0xaaaaaaaa
    dd 0xaaaaaaaa
current_esp:
    dd 0x0000
prot_esp:
    dd 0x0000

BIOS32_END:
