#include "hook.h"

#include <stddef.h>
#include <stdio.h>

#include <mach/mach.h>

void hook(void *target_func, void *hook_func)
{
  /* we are going to pray that a 32-bit displacement is going to be enough */
  /* there are multiple ways to ensure that the overwrite doesn't write where
   * you are currently executing (e.g. use a debugger and stop execution before
   * hooking) but we won't be doing that) */

  /* the jump instruction is 5 bytes 'e9 11 0f f5 37' and the lea instruction is
   * 4 bytes */
  ptrdiff_t d = hook_func - (target_func + 4 + 5);
  mach_error_t err;

  if (d > INT32_MAX || d < INT32_MIN) {
    fprintf(stderr, "we need 32-bit displacements for this to work, the given displacement is %p\n", (void *)d);
    return;
  }

  err = vm_protect(mach_task_self(), (vm_address_t)target_func + 4, 0x5, 0, VM_PROT_READ | VM_PROT_WRITE | VM_PROT_EXECUTE);
  if (err) {
    fprintf(stderr, "vm_protect failed: %s\n", mach_error_string(err));
    return;
  }

  /* write the bytes */
  // memset(old_plus, 0x90, 9);
  *(int8_t *)((uint64_t)target_func + 4) = 0xe9;
  *(int32_t *)((uint64_t)target_func + 4 + 1) = (int32_t)(d & 0xFFFFFFFF);

  printf("hooked!\n");
}
