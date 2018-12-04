#include "types.h"
#include "user.h"
#include "syscall.h"
#include "thread.h"

#define NUM_THREADS  4
#define INCR_PER_THREAD 5000
#define MONEY 100

int g_mutex;
int g_accounts[NUM_THREADS];

struct ctx {
  int tid;
  int amount;
} g_ctx[NUM_THREADS] = {};

void
worker_fn(void *ctxq)
{
  int i;
  int tmp;

  struct ctx *ctx = ctxq;
  int *src;
  int *dest;

  int pid = getpid();
  int tid = ctx->tid;

  src = &g_accounts[tid];
  dest = &g_accounts[(tid * 17) % NUM_THREADS];

  for (i = 0; i < 1000; i++) {
    mtx_lock(g_mutex);

    tmp = *src;
    *src = *dest;
    *dest = tmp;
  }

  printf(1, "[%d] done !!!\n", pid);
  exit();
}

int
worker_create(int tid)
{
  int pid;
  void *stack;
  struct ctx *ctx;

  printf(1, "worker_create: idx = %d\n", tid);

  stack = malloc(THREAD_STACK_SIZE);
  if (stack == 0) {
    printf(2, "malloc: failed to allocate thread stack\n");
    return -1;
  }
  memset(stack, 0, THREAD_STACK_SIZE);

  ctx = &g_ctx[tid];
  ctx->amount = MONEY;
  ctx->tid = tid;

  printf(1, "thread_create: begin\n");
  if ((pid = thread_create(worker_fn, THREAD_STACK(stack), ctx)) <= 0) {
    printf(2, "thread_create: failed %d\n", pid);
    return -1;
  }
  printf(1, "thread_create: success (pid = %d)\n", pid);

  return pid;
}


int
main(int argc, char *argv[])
{
  int i;
  int pid;
  int total;
  int tids[NUM_THREADS] = {};
  int exited = 0;
  void *stack;

  g_mutex = mtx_create(0);

  for (i = 0; i < NUM_THREADS; i++) {
    g_accounts[i] = MONEY;
  }

  for (i = 0; i < NUM_THREADS; i++) {
    printf(1, "[A] main: i = %d\n", i);
    pid = worker_create(i);
    tids[i] = pid;
    printf(1, "[B] main: i = %d\n", i);
    if (pid <= 0) {
      printf(2, "worker_create: failed %d\n", pid);
      return 1;
    }
  }

  while ((pid = thread_join(&stack)) != -1) {
//    free(stack);
    printf(1, "thread_join: exit %d\n", pid);
    exited++;
  }

  printf(1, "parent pid: %d\n", getpid());
  printf(1, "tids: \n");
  for (i = 0; i < NUM_THREADS; i++) {
    printf(1, "  [tid %d] = pid %d\n", i, tids[i]);
  }
  printf(1, "\n");

  total = 0;
  for (i = 0; i < NUM_THREADS; i++) {
    total += g_accounts[i];
  }

  printf(1, "exited    %d threads (expected %d threads)\n", exited, NUM_THREADS);
  printf(1, "got       n = %d\n", total);
  printf(1, "expected  n = %d\n", MONEY * NUM_THREADS);

  exit();
}
