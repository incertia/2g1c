#include <stdio.h>
#include <string.h>

#include <dlfcn.h>

#include "hook.h"
#include "hooks.h"
#include "haskell.h"

extern StgWord *stg_arg_stuff;

void install(void) __attribute__ ((constructor));
void install(void)
{
  void (*printRtsInfo)(void);
  void (*haskell_zp)(void);
  void ***numinteger;

  stg_regset_t regs = {};
  // void *mul = dlsym(RTLD_MAIN_ONLY, "base_GHCziNum_zt_info");
  // void *sub = dlsym(RTLD_MAIN_ONLY, "base_GHCziNum_zm_info");
  stg_arg_stuff = dlsym(RTLD_MAIN_ONLY, "stg_arg_bitmaps");
  numinteger = dlsym(RTLD_MAIN_ONLY, "base_GHCziNum_zdfNumInteger_closure");
  old_zp = dlsym(RTLD_MAIN_ONLY, "base_GHCziNum_zp_info");
  zp_closure = dlsym(RTLD_MAIN_ONLY, "base_GHCziNum_zp_closure");
  // haskell_zp = dlsym(RTLD_SELF, "UnsafeZZp_unsafezuzzpzq_info");
  haskell_zp = dlsym(RTLD_SELF, "UnsafeZZp_safezuweirdzuzzpzq_closure");
  // haskell_zp = dlsym(RTLD_SELF, "UnsafeZZp_unsafezuzzpzq_closure");
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

  if (memcmp((void *)((size_t)old_zp + 4), "\x4c\x39\xf8\x72", 3) == 0) {
    printf("found 'cmp rax, r15 -> jb ...'\n");
  }

  // try this weird thing where we replace NumInteger.plusInteger_closure with a
  // pointer to our own function
  // *(void **)((size_t)numinteger[1] & ~0x7) = *(void **)haskell_zp;
  ((void **)numinteger)[1] = haskell_zp;
  // we can also hook the Num.+ stub
  hook(old_zp, zp_stub);
}
