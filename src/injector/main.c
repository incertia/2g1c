#include "injector.h"

#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
  char *prog = argv[0];
  char *endptr;
  void *module;
  void *bootstrap;
  pid_t pid;

  if (argc != 3) {
    fprintf(stderr, "Usage: %s <pid> <lib>\n", prog);
    return 0;
  }

  char path[4096];
  realpath(argv[2], path);

  pid = strtol(argv[1], &endptr, 10);

  if (!(*argv[1] != 0 && *endptr == 0)) {
    fprintf(stderr, "invalid pid: %s\n", argv[1]);
    exit(1);
  }

  printf("pid: %u\n", pid);

  load(&module, &bootstrap);
  inject(pid, path, module, bootstrap);

  return 0;
}
