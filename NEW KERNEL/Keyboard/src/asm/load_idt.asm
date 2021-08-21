section .text
    global load_idt

load_idt:
    mov eax, [esp + 4]
    lidt [eax]
    ret
