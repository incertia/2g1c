#include "hooks.h"
#include "haskell.h"

#include <stdio.h>

void hk_zp()
{
  stg_regset_t regs;
  getregs(&regs);
  printf("---(+)---\n");
  printregs(&regs);

  print_closure(regs.r1.a, "R1", 0, &regs);
  print_closure(regs.r2.a, "R2", 0, &regs);
  print_closure(regs.r3.a, "R3", 0, &regs);
  print_closure(regs.r4.a, "R4", 0, &regs);
  print_closure(regs.r5.a, "R5", 0, &regs);
  print_closure(regs.r6.a, "R6", 0, &regs);
  print_closure(regs.sp, "*Sp", 0, &regs);
}
