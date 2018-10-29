#include "types.h"
#include "user.h"
#include "syscall.h"

static int foo = 10;

int
main(int argc, char *argv[])
{
  printf(1, "(virt) &foo  = 0x%p, (phys) &foo  = 0x%p\n", &foo, v2paddr(&foo));
  printf(1, "(virt) &main = 0x%p, (phys) &main = 0x%p\n", &main, v2paddr(&main));
  printf(1, "v2paddr calls = %d\n", getcount(SYS_v2paddr));

  exit();
}
