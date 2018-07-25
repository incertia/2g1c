global _zp_stub
global _zp_closure
global _old_zp

section .text

extern save_regs
extern restore_regs
extern _hk_zp

_zp_stub:
  cmp rax, r15
  jb _zp_stub.garbage
  call save_regs
  call _hk_zp
  call restore_regs
  push qword [rel _old_zp]
  add qword [rsp], 0x09
  ret
.garbage:
  mov rbx, [rel _zp_closure]
  jmp qword [r13 - 0x08]

section .bss

_zp_closure: resq 1
_old_zp: resq 1
