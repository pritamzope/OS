section .text
    extern isr_exception_handler
    global exception_0
    global exception_1
    global exception_2
    global exception_3
    global exception_4
    global exception_5
    global exception_6
    global exception_7
    global exception_8
    global exception_9
    global exception_10
    global exception_11
    global exception_12
    global exception_13
    global exception_14
    global exception_15
    global exception_16
    global exception_17
    global exception_18
    global exception_19
    global exception_20
    global exception_21
    global exception_22
    global exception_23
    global exception_24
    global exception_25
    global exception_26
    global exception_27
    global exception_28
    global exception_29
    global exception_30
    global exception_31
    global exception_128


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


exception_0:
    cli
    push byte 0     ; store default err code(0)
    push 0          ; push exception number index in IDT
    jmp exception_handler

exception_1:
    cli
    push byte 0     ; store default err code(0)
    push 1          ; push exception number index in IDT
    jmp exception_handler

exception_2:
    cli
    push byte 0     ; store default err code(0)
    push 2          ; push exception number index in IDT
    jmp exception_handler

exception_3:
    cli
    push byte 0     ; store default err code(0)
    push 3          ; push exception number index in IDT
    jmp exception_handler

exception_4:
    cli
    push byte 0     ; store default err code(0)
    push 4          ; push exception number index in IDT
    jmp exception_handler

exception_5:
    cli
    push byte 0     ; store default err code(0)
    push 5          ; push exception number index in IDT
    jmp exception_handler

exception_6:
    cli
    push byte 0     ; store default err code(0)
    push 6          ; push exception number index in IDT
    jmp exception_handler

exception_7:
    cli
    push byte 0     ; store default err code(0)
    push 7          ; push exception number index in IDT
    jmp exception_handler

exception_8:
    cli
    push 8          ; push exception number index in IDT
    jmp exception_handler

exception_9:
    cli
    push byte 0     ; store default err code(0)
    push 9          ; push exception number index in IDT
    jmp exception_handler

exception_10:
    cli
    push 10          ; push exception number index in IDT
    jmp exception_handler

exception_11:
    cli
    push 11          ; push exception number index in IDT
    jmp exception_handler

exception_12:
    cli
    push 12          ; push exception number index in IDT
    jmp exception_handler

exception_13:
    cli
    push 13          ; push exception number index in IDT
    jmp exception_handler

exception_14:
    cli
    push 14          ; push exception number index in IDT
    jmp exception_handler

exception_15:
    cli
    push byte 0     ; store default err code(0)
    push 15          ; push exception number index in IDT
    jmp exception_handler

exception_16:
    cli
    push byte 0     ; store default err code(0)
    push 16          ; push exception number index in IDT
    jmp exception_handler

exception_17:
    cli
    push byte 0     ; store default err code(0)
    push 17          ; push exception number index in IDT
    jmp exception_handler

exception_18:
    cli
    push byte 0     ; store default err code(0)
    push 18          ; push exception number index in IDT
    jmp exception_handler

exception_19:
    cli
    push byte 0     ; store default err code(0)
    push 19          ; push exception number index in IDT
    jmp exception_handler

exception_20:
    cli
    push byte 0     ; store default err code(0)
    push 20          ; push exception number index in IDT
    jmp exception_handler

exception_21:
    cli
    push byte 0     ; store default err code(0)
    push 21          ; push exception number index in IDT
    jmp exception_handler

exception_22:
    cli
    push byte 0     ; store default err code(0)
    push 22          ; push exception number index in IDT
    jmp exception_handler

exception_23:
    cli
    push byte 0     ; store default err code(0)
    push 23          ; push exception number index in IDT
    jmp exception_handler

exception_24:
    cli
    push byte 0     ; store default err code(0)
    push 24          ; push exception number index in IDT
    jmp exception_handler

exception_25:
    cli
    push byte 0     ; store default err code(0)
    push 25          ; push exception number index in IDT
    jmp exception_handler

exception_26:
    cli
    push byte 0     ; store default err code(0)
    push 26          ; push exception number index in IDT
    jmp exception_handler

exception_27:
    cli
    push byte 0     ; store default err code(0)
    push 27          ; push exception number index in IDT
    jmp exception_handler

exception_28:
    cli
    push byte 0     ; store default err code(0)
    push 28          ; push exception number index in IDT
    jmp exception_handler

exception_29:
    cli
    push byte 0     ; store default err code(0)
    push 29          ; push exception number index in IDT
    jmp exception_handler

exception_30:
    cli
    push byte 0     ; store default err code(0)
    push 30          ; push exception number index in IDT
    jmp exception_handler

exception_31:
    cli
    push byte 0     ; store default err code(0)
    push 31          ; push exception number index in IDT
    jmp exception_handler

exception_128:
    cli
    push byte 0     ; store default err code(0)
    push 128          ; push exception number index in IDT
    jmp exception_handler


