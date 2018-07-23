#ifndef __INJECTOR_H__
#define __INJECTOR_H__

#include "mach_inject.h"

void load(void **module, void **bootstrapfn);
void inject(pid_t pid, const char* lib, void *module, void *bootstrapfn);

#endif
