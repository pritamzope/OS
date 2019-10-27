.section .text
.global load_idt

load_idt:
	mov 4(%esp), %eax
	lidt (%eax)
  ret
