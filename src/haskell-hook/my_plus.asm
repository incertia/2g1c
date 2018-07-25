global _my_plus
global _zp_closure
; global _old_plus

section .text

extern _hk_plus
extern _old_plus
extern save_regs
extern restore_regs

_my_plus:
  ; we place the hook after the following instruction, so we don't actually need
  ; to perform it and then maybe the hook technique can generalize
  ; lea rax, [rbp - 0x08]
  cmp rax, r15
  jb  _my_plus.garbage

  ; now that we've taken care of the stack garbage collector, we can save all
  ; the registers, call a C function, and then jump back to the original
  ; function
  call save_regs
  call _hk_plus
  call restore_regs

  ; now we jump back to the original function
  ; this is a hack to avoid messing with any registers so we can guarantee that
  ; the state of the STG machine is not tampered with
  push qword [rel _old_plus]
  add qword [rsp], 0x9
  ret
.garbage:
  ; we need to load rbx with _base_GHCziNum_zp_closure, which is kind of
  ; difficult unless we have an extra variable to read
  mov rbx, [rel _zp_closure]
  jmp qword [r13 - 0x08]

section .bss

_zp_closure: resq 1
