section .text
    global load_gdt
    global detect_v86
    global enter_v86

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


detect_v86:
   smsw    ax
   and     eax,1           ;CR0.PE bit
   ret

; you should declare this function as :
; extern void enter_v86(uint32_t ss, uint32_t esp, uint32_t cs, uint32_t eip);
enter_v86:
   mov ebp, esp               ; save stack pointer

   push dword  [ebp+4]        ; ss
   push dword  [ebp+8]        ; esp
   pushfd                     ; eflags
   or dword [esp], (1 << 17)  ; set VM flags
   push dword [ebp+12]        ; cs
   push dword  [ebp+16]       ; eip
   iret
