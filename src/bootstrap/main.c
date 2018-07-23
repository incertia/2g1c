#include <mach/mach.h>
#include <pthread.h>
#include <dlfcn.h>
#include <stddef.h>

#include <stdio.h>

#define DLLEXPORT __attribute__((visibility("default")))

extern void _pthread_set_self(void*);

DLLEXPORT void
bootstrap(ptrdiff_t offset, void *param, size_t psize, void *dummy);

void *loader_thread(void *lib)
{
  void *module = dlopen((char *)lib, RTLD_NOW);
  if (!module) {
    fprintf(stderr, "failed to open library: %s\n", dlerror());
  }
  return 0;
}

void bootstrap(ptrdiff_t offset, void *param, size_t psize, void *dummy)
{
  _pthread_set_self(dummy);

  pthread_attr_t attr;
  pthread_attr_init(&attr);

  int policy;
  pthread_attr_getschedpolicy(&attr, &policy);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
  pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);

  struct sched_param sched;
  sched.sched_priority = sched_get_priority_max(policy);
  pthread_attr_setschedparam(&attr, &sched);

  pthread_t thread;
  pthread_create(&thread, &attr,
                 loader_thread,
                 (void *)param);
  pthread_attr_destroy(&attr);

  thread_suspend(mach_thread_self());
}
