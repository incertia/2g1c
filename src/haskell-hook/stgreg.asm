; this is some assembly code to retrieve some haskell registers, which have the
; following assignments:
; BaseReg   -> r13
; Sp        -> rbp
; Hp        -> r12
; R1        -> rbx
; R2        -> r14
; R3        -> rsi
; R4        -> rdi
; R5        -> r8
; R6        -> r9
; SpLim     -> r15
; MachSp    -> rsp
; F|D[1..6] -> xmm[1..6]

global _getregs
global save_regs
global restore_regs

extern _memcpy

section .text

save_regs:
  mov qword [rel stg_regs + 0x00], r13 ; BaseReg
  mov qword [rel stg_regs + 0x08], rbp ; Sp
  mov qword [rel stg_regs + 0x10], r12 ; Hp
  mov qword [rel stg_regs + 0x18], rbx ; R1
  mov qword [rel stg_regs + 0x20], r14 ; R2
  mov qword [rel stg_regs + 0x28], rsi ; R3
  mov qword [rel stg_regs + 0x30], rdi ; R4
  mov qword [rel stg_regs + 0x38], r8  ; R5
  mov qword [rel stg_regs + 0x40], r9  ; R6
  mov qword [rel stg_regs + 0x48], r15 ; SpLim
  lea rax, [rsp + 8]
  mov qword [rel stg_regs + 0x50], rax ; MachSp
  ret

restore_regs:
  mov qword r13, [rel stg_regs + 0x00] ; BaseReg
  mov qword rbp, [rel stg_regs + 0x08] ; Sp
  mov qword r12, [rel stg_regs + 0x10] ; Hp
  mov qword rbx, [rel stg_regs + 0x18] ; R1
  mov qword r14, [rel stg_regs + 0x20] ; R2
  mov qword rsi, [rel stg_regs + 0x28] ; R3
  mov qword rdi, [rel stg_regs + 0x30] ; R4
  mov qword r8,  [rel stg_regs + 0x38] ; R5
  mov qword r9,  [rel stg_regs + 0x40] ; R6
  mov qword r15, [rel stg_regs + 0x48] ; SpLim
  ; don't do MachSp because it should be right
  ret

_getregs:
  ; rdi is a pointer to stg_regset_t
  lea rsi, [rel stg_regs]
  mov rdx, 0x58
  call _memcpy
  ret

section .bss
stg_regs: resq 11
