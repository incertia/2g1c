#include "injector.h"
#include "mach_inject.h"

#include <stdio.h>
#include <string.h>

#include <mach/mach.h>
#include <mach/thread_act.h>
#include <mach/mach_init.h>
#include <pthread.h>
#include <dlfcn.h>
#include <signal.h>
#include <sys/proc_info.h>
#include <libproc.h>

void load(void **module, void **bootstrapfn)
{
  *module = dlopen("libbootstrap.dylib", RTLD_NOW | RTLD_LOCAL);

  printf("module: %p\n", module);
  if (!*module)
  {
    fprintf(stderr, "dlopen error: %s\n", dlerror());
    return;
  }

  *bootstrapfn = dlsym(*module, "bootstrap");
  printf("bootstrapfn: %p\n", *bootstrapfn);

  if (!*bootstrapfn)
  {
    fprintf(stderr, "could not locate bootstrap fn\n");
    return;
  }
}

void inject(pid_t pid, const char* lib, void *module, void *bootstrapfn)
{
  if (!module || !bootstrapfn) {
    fprintf(stderr, "tried injecting without loading :(\n");
    return;
  }

  mach_error_t err = mach_inject((mach_inject_entry)bootstrapfn, lib, strlen(lib) + 1, pid, 0);
}
