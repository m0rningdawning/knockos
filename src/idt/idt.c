#include "../../include/idt.h"
#include "../../include/config.h"

struct idt_desc idt_descriptors[KNOCKOS_TOTAL_INTS];
struct idtr_desc idtr_descriptors;

void idt_init() {

}

