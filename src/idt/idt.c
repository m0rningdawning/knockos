#include "../../include/idt.h"
#include "../../include/config.h"
#include "../../include/io.h"
#include "../../include/kernel.h"
#include "../../include/memory.h"

struct idt_desc idt_descriptors[KNOCKOS_TOTAL_INTS];
struct idtr_desc idtr_descriptors;

extern void idt_load(struct idtr_desc *ptr);
extern void int21h();
extern void no_int();

void int21h_handler() {
  printf("Keyboard pressed!\n");
  outb(0x20, 0x20);
}

void no_int_handler() { outb(0x20, 0x20); }

void idt_zero() { printf("Divide by zero error\n"); }

void idt_set(int int_no, void *address) {
  struct idt_desc *desc = &idt_descriptors[int_no];
  desc->offset_1 = (uint32_t)address & 0x0000ffff;
  desc->selector = KERNEL_CODE_SELECTOR;
  desc->zero = 0x00;
  desc->type_attr = 0xEE;
  desc->offset_2 = (uint32_t)address >> 16;
}

void idt_init() {
  memset(idt_descriptors, 0, sizeof(idt_descriptors));
  idtr_descriptors.limit = sizeof(idt_descriptors) - 1;
  idtr_descriptors.base = (uint32_t)idt_descriptors;

  for (int i = 0; i < KNOCKOS_TOTAL_INTS; ++i) {
    idt_set(i, no_int);
  }

  idt_set(0, idt_zero);
  idt_set(0x21, int21h);

  // Load the idt
  idt_load(&idtr_descriptors);
}
