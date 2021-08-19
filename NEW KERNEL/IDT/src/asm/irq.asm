section .text
    extern isr_irq_handler

irq_handler:
    pusha                 ; push all registers
    mov ax, ds
    push eax              ; save ds

    mov ax, 0x10          ; load kernel data segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push esp
    call isr_irq_handler
    pop esp

    pop ebx                ; restore kernel data segment
    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx

    popa                ; restore all registers
    add esp, 0x8        ; restore stack for erro no been pushed

    sti                 ; re-enable interrupts
    iret


%macro ADD_IRQ 2
  global isr_%1
  irq_%1:
    cli
    push 0
    push %2
    jmp irq_handler
%endmacro


ADD_IRQ 0, 32
ADD_IRQ 1, 33
ADD_IRQ 2, 34
ADD_IRQ 3, 35
ADD_IRQ 4, 36
ADD_IRQ 5, 37
ADD_IRQ 6, 38
ADD_IRQ 7, 39
ADD_IRQ 8, 40
ADD_IRQ 9, 41
ADD_IRQ 10, 42
ADD_IRQ 11, 43
ADD_IRQ 12, 45
ADD_IRQ 13, 46
ADD_IRQ 14, 47
ADD_IRQ 15, 48

