#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <dlfcn.h>

#include "hook.h"
#include "hooks.h"
#include "haskell.h"

#include <mach/mach.h>

extern StgWord *stg_arg_stuff;

extern void __stginit_UnsafeZZp(void);
extern void hs_init(int *argc, char ***argv);

extern int maybePerformBlockedException(void *, void *);

void install(void) __attribute__ ((constructor));
void install(void)
{
  void (*printRtsInfo)(void);
  void (*haskell_zp)(void);
  void ***numinteger;
  void *end_tso1;
  void *end_tso2;
  void *async;
  void *maybe;
  // uint32_t offset;
  void **tso_ptr;
  void *my_putstr;
  void *foreign_putstr;
  uint32_t tso_offset;

  stg_regset_t regs = {};
  int argc = 1;
  char **argv = malloc(sizeof(char *));
  argv[0] = "yoloswag";

  // initialize the rts of our internal haskell module
  hs_init(&argc, &argv);

  // void *mul = dlsym(RTLD_MAIN_ONLY, "base_GHCziNum_zt_info");
  // void *sub = dlsym(RTLD_MAIN_ONLY, "base_GHCziNum_zm_info");
  stg_arg_stuff = dlsym(RTLD_MAIN_ONLY, "stg_arg_bitmaps");
  numinteger = dlsym(RTLD_MAIN_ONLY, "base_GHCziNum_zdfNumInteger_closure");
  end_tso1 = dlsym(RTLD_MAIN_ONLY, "stg_END_TSO_QUEUE_closure");
  end_tso2 = dlsym(RTLD_SELF, "stg_END_TSO_QUEUE_closure");
  async = dlsym(RTLD_SELF, "stg_unmaskAsyncExceptionszh_ret_info");
  // maybe = dlsym(RTLD_SELF, "maybePerformBlockedException");
  maybe = maybePerformBlockedException;

  printf("end_tso1: %p\n", end_tso1);
  printf("end_tso2: %p\n", end_tso2);
  printf("maybe: %p\n", maybe);

  tso_ptr = (void **)((uint64_t)async + 46 + *(uint32_t *)((uint64_t)async + 42));
  *tso_ptr = end_tso1;
  // we need to replace some bytes of of our maybePerformBlockedException with
  // the correct offset to the original binary's stg_END_TSO_QUEUE_closure
  vm_protect(mach_task_self(), (vm_address_t)maybe + 35, 0x4, 0, VM_PROT_READ |
             VM_PROT_WRITE | VM_PROT_EXECUTE);
  tso_offset = (uint32_t)((uint64_t)end_tso1 - ((uint64_t)maybe + 39));
  *(uint32_t *)((uint64_t)maybe + 35) = tso_offset;

  old_zp = dlsym(RTLD_MAIN_ONLY, "base_GHCziNum_zp_info");
  zp_closure = dlsym(RTLD_MAIN_ONLY, "base_GHCziNum_zp_closure");
  haskell_zp = dlsym(RTLD_SELF, "UnsafeZZp_safezuweirdzuzzpzq_closure");
  haskell_zp = dlsym(RTLD_SELF, "UnsafeZZp_unsafezuzzpzqzqzq_closure");
  my_putstr = dlsym(RTLD_SELF, "base_GHCziIOziHandleziText_hPutStr2_info");
  foreign_putstr = dlsym(RTLD_MAIN_ONLY, "base_GHCziIOziHandleziText_hPutStr2_info");
  printf("stg_arg_bitmaps: %p\n", stg_arg_stuff);
  printf("NumInteger_closure: %p\n", numinteger);
  printf("base GHC.Num +: %p\n", old_zp);
  printf("base GHC.Num + (closure): %p\n", zp_closure);
  printf("zp_stub: %p\n", zp_stub);
  printf("hk_zp: %p\n", hk_zp);
  printf("haskell_zp: %p\n", haskell_zp);

  print_function((StgPtr)zp_closure, "base GHC.Num +", 0, &regs);
  print_function((StgPtr)numinteger[1], "plusInteger", 0, &regs);
  print_function((StgPtr)haskell_zp, "haskell_zp", 0, &regs);

  printRtsInfo = dlsym(RTLD_MAIN_ONLY, "printRtsInfo");
  if (printRtsInfo) {
    printRtsInfo();
  }

  if (memcmp((void *)((uint64_t)old_zp + 4), "\x4c\x39\xf8\x72", 3) == 0) {
    printf("found 'cmp rax, r15 -> jb ...'\n");
  }

  // try this weird thing where we replace NumInteger.plusInteger_closure with a
  // pointer to our own function
  // *(void **)((uint64_t)numinteger[1] & ~0x7) = *(void **)haskell_zp;
  ((void **)numinteger)[1] = haskell_zp;
  // we can also hook the Num.+ stub
  hook(old_zp, zp_stub);
  // hook(my_putstr, foreign_putstr);
}
