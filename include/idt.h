#ifndef IDT_H
#define IDT_H

#include <stdint.h>

// OSDev.org IDT
struct idt_desc {
  uint16_t offset_1;
  uint16_t selector;
  uint8_t zero;
  uint8_t type_attr;
  uint16_t offset_2;
} __attribute__((packed));

// IDT Register
struct idtr_desc {
  uint16_t limit;
  uint32_t base;
} __attribute__((packed));

void idt_zero();
void idt_set(int int_no, void *address);
void idt_init();
void enable_int();
void disable_int();

#endif /* ifndef IDT_H */
