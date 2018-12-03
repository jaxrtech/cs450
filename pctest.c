#include "types.h"
#include "user.h"
#include "syscall.h"
#include "thread.h"

int foo = 10;

void
worker(void *ctx)
{
  printf(1, "Got %d\n", (uint) ctx);
  printf(1, "foo = %d\n", foo);
  printf(1, "exit: child\n");
  exit();
}

uint *tstack;

int
main(int argc, char *argv[])
{
  int pid;
  int wait_pid;
  void *stack;

  stack = malloc(THREAD_STACK_SIZE);
  if (stack == 0) {
    printf(2, "malloc: failed to allocate thread stack\n");
    return 1;
  }

  printf(1, "malloc: tstack = %p\n", tstack);

  if ((pid = thread_create(worker, THREAD_STACK(tstack), (void *) 42)) <= 0) {
    printf(2, "thread_create: failed %d\n", pid);
    return 1;
  }
  printf(1, "thread_create: tid = %d\n", pid);

  wait_pid = thread_join(&stack);
  if (wait_pid > 0) {
    free(stack);
  }

  printf(1, "wait: child thread exited\n");
  printf(1, "exit: parent\n");

  exit();
}
