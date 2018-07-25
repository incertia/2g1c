#include <stdio.h>
#include <string.h>

#include <dlfcn.h>

#include "hook.h"
#include "hooks.h"
#include "haskell.h"

void install(void) __attribute__ ((constructor));
void install(void)
{
  void (*printRtsInfo)(void);
  // void *mul = dlsym(RTLD_MAIN_ONLY, "base_GHCziNum_zt_info");
  // void *sub = dlsym(RTLD_MAIN_ONLY, "base_GHCziNum_zm_info");
  old_zp = dlsym(RTLD_MAIN_ONLY, "base_GHCziNum_zp_info");
  zp_closure = dlsym(RTLD_MAIN_ONLY, "base_GHCziNum_zp_closure");
  printf("base GHC.Num +: %p\n", old_zp);
  printf("base GHC.Num + (closure): %p\n", zp_closure);
  printf("zp_stub: %p\n", zp_stub);
  printf("hk_zp: %p\n", hk_zp);

  printRtsInfo = dlsym(RTLD_MAIN_ONLY, "printRtsInfo");
  if (printRtsInfo) {
    printRtsInfo();
  }

  if (memcmp((void *)((size_t)old_zp + 4), "\x4c\x39\xf8\x72", 3) == 0) {
    printf("found 'cmp rax, r15 -> jb ...'\n");
  }

  hook(old_zp, zp_stub);
  // troll
  // hook(mul, sub);
}
