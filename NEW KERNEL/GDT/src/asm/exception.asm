section .text
    extern isr_exception_handler

exception_handler:
    pusha                 ; push all registers
    mov ax, ds
    push eax              ; save ds
    
    mov ax, 0x10          ; load kernel data segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call isr_exception_handler

    pop ebx             ; restore kernel data segment
    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx

    popa                ; restore all registers
    add esp, 0x8        ; restore stack for erro no been pushed

    sti                 ; re-enable interrupts
    iret


; Macro for interrupt service routines that do push an error code
%macro ISR_ERRCODE 1
global exception_%1
exception_%1:
    ; We don't need to push a byte, the CPU did it for us
    push %1
    jmp exception_handler
%endmacro

; Macro for interrupt service routines that don't push an error code
%macro ISR_NOERRCODE 1
global exception_%1
exception_%1:
    ; There is no error code
    push byte 0         ; Push a default error code as REGISTERS expects it
    push %1             ; Push the exception index
    jmp exception_handler
%endmacro


ISR_NOERRCODE   0       ; Divison by zero exception
ISR_NOERRCODE   1       ; Debug exception
ISR_NOERRCODE   2       ; Non-maskable interrupt
ISR_NOERRCODE   3       ; INT3 breakpoint
ISR_NOERRCODE   4       ; Overflow exception
ISR_NOERRCODE   5       ; Bound exception
ISR_NOERRCODE   6       ; Invalid opcode
ISR_NOERRCODE   7       ; No FPU
ISR_ERRCODE     8       ; Double fault
ISR_NOERRCODE   9       ; Coprocessor segment overrun (reserved now)
ISR_ERRCODE     10      ; Invalid TSS
ISR_ERRCODE     11      ; Segment not present
ISR_ERRCODE     12      ; Stack segment exception
ISR_ERRCODE     13      ; General protection fault
ISR_ERRCODE     14      ; Page fault
ISR_NOERRCODE   15      ; Reserved exception
ISR_NOERRCODE   16      ; Floating-point exception
ISR_ERRCODE     17      ; Alignment check
ISR_NOERRCODE   18      ; Machine check
ISR_NOERRCODE   19      ; SIMD floating-point exception
ISR_NOERRCODE   20      ; Virtualization exception
ISR_ERRCODE     21      ; Control-protection exception
; 22 - 27 are reserved
ISR_NOERRCODE   28      ; Hypervisor injection exception
ISR_ERRCODE     29      ; VMM communication exception
ISR_ERRCODE     30      ; Security exception
ISR_NOERRCODE   31      ; Reserved

ISR_NOERRCODE   128     ; System call