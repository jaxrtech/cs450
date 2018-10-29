#include "types.h"
#include "user.h"
#include "syscall.h"

int foo = 10;

int
main(int argc, char *argv[])
{
  int a = 42;
  void *bad = (void*) 0x99999999;

  printf(1, "[global]  (virt) &foo = 0x%p, (phys) &foo = 0x%p\n", &foo, v2paddr(&foo));
  printf(1, "[fn]      (virt) &main = 0x%p, (phys) &main = 0x%p\n", &main, v2paddr(&main));
  printf(1, "[stack]   (virt) &a = 0x%p, (phys) &a = 0x%p\n", &a, v2paddr(&a));
  printf(1, "[invalid] (virt) bad = 0x%p, (phys) bad = 0x%p\n", bad, v2paddr(bad));
  printf(1, "v2paddr calls = %d\n", getcount(SYS_v2paddr));

  exit();
}
