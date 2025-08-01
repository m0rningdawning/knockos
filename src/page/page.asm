[BITS 32]

section .asm

global page_load_pd_entry
global enable_paging

page_load_pd_entry:
  push ebp
  mov ebp, esp
  mov eax, [ebp+8]
  mov cr3, eax
  pop ebp
  ret

enable_paging:
  push ebp
  mov ebp, esp
  mov eax, cr0
  or eax, 0x80000000
  mov cr0, eax
  pop ebp
  ret

