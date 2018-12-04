#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}


int
sys_thread_create(void)
{
  void *tmain;
  void *stack;
  void *arg;

  if (argptr(0, (void *) &tmain, sizeof(tmain)) < 0) {
    return -1;
  }

  if (argint(1, (void *) &stack) < 0) {
    return -2;
  }

  if (argint(2, (void *) &arg) < 0) {
    return -3;
  }

  return thread_create(tmain, stack, arg);
}


int
sys_thread_join(void)
{
  void **stack;

  if (argptr(0, (void *) &stack, sizeof(stack))) {
    return -1;
  }

  return thread_join(stack);
}


int
sys_mtx_create(void)
{
  int locked;

  if (argint(0, (void *) &locked)) {
    return -1;
  }

  return mtx_create(locked);
}


int
sys_mtx_lock(void)
{
  int lock_id;

  if (argint(0, (void *) &lock_id)) {
    return -1;
  }

  return mtx_lock(lock_id);
}


int
sys_mtx_unlock(void)
{
  int lock_id;

  if (argint(0, (void *) &lock_id)) {
    return -1;
  }

  return mtx_unlock(lock_id);
}