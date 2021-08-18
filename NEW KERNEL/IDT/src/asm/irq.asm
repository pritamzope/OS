section .text
    extern isr_irq_handler
    global irq_0
    global irq_1
    global irq_2
    global irq_3
    global irq_4
    global irq_5
    global irq_6
    global irq_7
    global irq_8
    global irq_9
    global irq_10
    global irq_11
    global irq_12
    global irq_13
    global irq_14
    global irq_15


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



irq_0:
    cli
    push byte 0         ; store default err code(0)
    push byte 32        ; push irq number index in IDT
    jmp irq_handler

irq_1:
    cli
    push byte 0         ; store default err code(0)
    push byte 33        ; push irq number index in IDT
    jmp irq_handler

irq_2:
    cli
    push byte 0         ; store default err code(0)
    push byte 34        ; push irq number index in IDT
    jmp irq_handler

irq_3:
    cli
    push byte 0         ; store default err code(0)
    push byte 35        ; push irq number index in IDT
    jmp irq_handler

irq_4:
    cli
    push byte 0         ; store default err code(0)
    push byte 36        ; push irq number index in IDT
    jmp irq_handler

irq_5:
    cli
    push byte 0         ; store default err code(0)
    push byte 37        ; push irq number index in IDT
    jmp irq_handler

irq_6:
    cli
    push byte 0         ; store default err code(0)
    push byte 38        ; push irq number index in IDT
    jmp irq_handler

irq_7:
    cli
    push byte 0         ; store default err code(0)
    push byte 39        ; push irq number index in IDT
    jmp irq_handler

irq_8:
    cli
    push byte 0         ; store default err code(0)
    push byte 40        ; push irq number index in IDT
    jmp irq_handler

irq_9:
    cli
    push byte 0         ; store default err code(0)
    push byte 41        ; push irq number index in IDT
    jmp irq_handler

irq_10:
    cli
    push byte 0         ; store default err code(0)
    push byte 42        ; push irq number index in IDT
    jmp irq_handler

irq_11:
    cli
    push byte 0         ; store default err code(0)
    push byte 43        ; push irq number index in IDT
    jmp irq_handler

irq_12:
    cli
    push byte 0         ; store default err code(0)
    push byte 44        ; push irq number index in IDT
    jmp irq_handler

irq_13:
    cli
    push byte 0         ; store default err code(0)
    push byte 45        ; push irq number index in IDT
    jmp irq_handler

irq_14:
    cli
    push byte 0         ; store default err code(0)
    push byte 46        ; push irq number index in IDT
    jmp irq_handler

irq_15:
    cli
    push byte 0         ; store default err code(0)
    push byte 47        ; push irq number index in IDT
    jmp irq_handler


