#ifndef ISR_H
#define ISR_H

#include "types.h"

struct registers
{
  uint32 ds;
  uint32 edi, esi, ebp, esp, ebx, edx, ecx, eax;  // pushed by pusha
  uint32 int_no, err_code;  // interrupt number and error code
  uint32 eip, cs, eflags, useresp, ss;  // pushed by the processor automatically
};

extern void isr_0();
extern void isr_1();
extern void isr_2();
extern void isr_3();
extern void isr_4();
extern void isr_5();
extern void isr_6();
extern void isr_7();
extern void isr_8();
extern void isr_9();
extern void isr_10();
extern void isr_11();
extern void isr_12();
extern void isr_13();
extern void isr_14();
extern void isr_15();

extern void common_isr_stub_handler();
extern void isr_handler(struct registers);

extern void isr_0_handler();
extern void isr_1_handler();
extern void isr_2_handler();
extern void isr_3_handler();
extern void isr_4_handler();
extern void isr_5_handler();
extern void isr_6_handler();
extern void isr_7_handler();
extern void isr_8_handler();
extern void isr_9_handler();
extern void isr_10_handler();
extern void isr_11_handler();
extern void isr_12_handler();
extern void isr_13_handler();
extern void isr_14_handler();
extern void isr_15_handler();

#endif
