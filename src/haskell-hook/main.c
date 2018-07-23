#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <dlfcn.h>

#include <mach/mach.h>

#include "haskell.h"

void *old_plus;
extern void my_plus();

void
print_closure(StgPtr closure,
              const char *name,
              int indent,
              stg_regset_t *regs)
{
  uint64_t tag = (uint64_t)closure & 0x7;
  StgClosure *c = (StgClosure *)((uint64_t)closure & ~0x7);
  StgPtr _c = (StgPtr)c;
  const char *where;
  const StgInfoTable *i;
  char fmt[4096];
  StgHalfWord ctype;

  if (!closure) {
    snprintf(fmt, 4096, "%%%ds%s: null pointer\n", indent, name);
    fprintf(stderr, fmt, "");
    return;
  }

  if (_c >= regs->sp) {
    where = "STACK";
  } else if (_c <= regs->hp &&
             _c >= regs->basereg->rCurrentNursery->start) {
    where = "HEAP";
  } else {
    where = "???";
  }

  i = c->header.info; 
  snprintf(fmt, 4096, "%%%ds%s: %s closure at %p...\n", indent, name, where,
           closure);
  printf(fmt, "");
  snprintf(fmt, 4096, "%%%ds%s: tag=%llx\n", indent, name, tag);
  printf(fmt, "");
  snprintf(fmt, 4096, "%%%ds%s: real addr=%p\n", indent, name, c);
  printf(fmt, "");
  snprintf(fmt, 4096, "%%%ds%s: info ptr=%p\n", indent, name, i);
  printf(fmt, "");
  i--;
  snprintf(fmt, 4096, "%%%ds%s: info tbl=%p\n", indent, name, i);
  printf(fmt, "");

  ctype = i->type;

  snprintf(fmt, 4096, "%%%ds%s: closure type: %s\n", indent, name,
           closure_type_str(ctype));
  printf(fmt, "");

  if (ctype >= N_CLOSURE_TYPES) {
    fprintf(stderr, "not a valid closure\n");
    return;
  }

  if (pointers_first(ctype)) {
    int j;
    int n;
    snprintf(fmt, 4096, "%%%ds%s: closure payload pointers: %u\n", indent, name,
             i->layout.payload.ptrs);
    printf(fmt, "");
    snprintf(fmt, 4096, "%%%ds%s: closure payload non-pointers: %u\n", indent,
             name, i->layout.payload.nptrs);
    printf(fmt, "");

    n = i->layout.payload.ptrs + i->layout.payload.nptrs;

    for (j = 0; j < n; j++) {
      snprintf(fmt, 4096, "%%%ds%s: payload[%d]=%p\n", indent, name, j,
               c->payload[j]);
      printf(fmt, "");
      // if (j < i->layout.payload.ptrs) {
      //   print_closure((StgPtr)c->payload[j], "PAYLOADPTR", indent + 2, regs);
      // }
    }
  } else if (bitmap(ctype)) {
    snprintf(fmt, 4096, "%%%ds%s: closure layout: 0x%llx\n", indent, name,
             i->layout.bitmap);
    printf(fmt, "");
  } else {
    snprintf(fmt, 4096, "%%%ds%s: print_closure: unknown layout field format: "
             "%d\n", indent, name, ctype);
    fprintf(stderr, fmt, "");
  }
}

void printregs(stg_regset_t *regs)
{
  printf("BaseReg=%p\n", (void *)regs->basereg);
  printf("Sp=%p\n", regs->sp);
  printf("SpLim=%p\n", regs->splim);
  printf("Hp=%p\n", regs->hp);
  printf("HpLim=%p\n", regs->basereg->rHpLim);
  printf("R1=%p\n", regs->r1.a);
  printf("R2=%p\n", regs->r2.a);
  printf("R3=%p\n", regs->r3.a);
  printf("R4=%p\n", regs->r4.a);
  printf("R5=%p\n", regs->r5.a);
  printf("R6=%p\n", regs->r6.a);
  printf("MachSp=%p\n", regs->machsp);
}

void hk_plus()
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

  err = vm_protect(mach_task_self(), (vm_address_t)target_func, 0x5, 0, VM_PROT_READ | VM_PROT_WRITE | VM_PROT_EXECUTE);
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

void install(void) __attribute__ ((constructor));
void install(void)
{
    old_plus = dlsym(RTLD_MAIN_ONLY, "base_GHCziNum_zp_info");
    printf("base GHC.Num +: %p\n", old_plus);
    printf("my_plus: %p\n", my_plus);
    printf("hk_plus: %p\n", hk_plus);

    if (memcmp((void *)((size_t)old_plus + 4), "\x4c\x39\xf8\x72", 3) == 0) {
      printf("found 'cmp rax, r15 -> jb ...'\n");
    }

    hook(old_plus, my_plus);

    // Use this to discover a pointer to the function we want to hijack
    // _dyld_lookup_and_bind(
    //     "_print_hw",
    //     (void**) &victim_func_ptr,
    //     NULL);

    // //TODO check for bad victim_func_ptr

    // // Assign our long pointer to our function prototype
    // victim_func = (void (*)())victim_func_ptr;

    // printf("testlib: victim_func_ptr   = %ld\n", (long)victim_func_ptr);
    // printf("testlib: victim_func       = %ld\n", (long)victim_func);

    // // Do the override
    // mach_error_t me;

    // me = mach_override_ptr(
    //                   victim_func_ptr,
    //                   (void*)&my_print_hw,
    //                   (void**)&victim_func);
}
