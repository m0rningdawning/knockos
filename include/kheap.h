#ifndef KHEAP_H_
#define KHEAP_H_

#include <stddef.h>
#include <stdint.h>

void kheap_init();
void* kmalloc(size_t size);
void* kzalloc(size_t size);
void kfree(void* p);

#endif /* ifndef KHEAP_H_ */
