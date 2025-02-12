#include "kernel/types.h"
#include "kernel/x86.h"
#include "kernel/defs.h"
#include "kernel/date.h"
#include "kernel/param.h"
#include "kernel/memlayout.h"
#include "kernel/mmu.h"
#include "kernel/proc.h"
#include "kernel/syscall.h"

// OUR FILE
#include "kernel/strace.h"

int sys_fork(void) { return fork(); }

int sys_exit(void) {
  exit();
  return 0; // not reached
}

int sys_wait(void) { return wait(); }

int sys_kill(void) {
  int pid;

  if (argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int sys_getpid(void) { return proc->pid; }

int sys_sbrk(void) {
  int addr;
  int n;

  if (argint(0, &n) < 0)
    return -1;
  addr = proc->sz;
  if (growproc(n) < 0)
    return -1;
  return addr;
}

int sys_sleep(void) {
  int n;
  uint ticks0;

  if (argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while (ticks - ticks0 < n) {
    if (proc->killed) {
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
int sys_uptime(void) {
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

// strace system call accepts integer to set process traced value
int sys_strace(void) {
	int input;
	argint(0, &input);
	struct proc *target_proc = proc;
	target_proc->traced = (input & 1) ? input : 0;
	return 0;
}

//dump system call accepts char* pointer and integer index
int sys_dump(void){
    char* temp;
    int input_idx;
    argptr(0, &temp, sizeof(*temp));
    argint(1, &input_idx);
    return dump(temp, input_idx);
}

int sys_set_call_flag(void){
    int input;
    argint(0, &input);
    return set_call_flag(input);
}

int sys_getN(void){
    return N;
}

int sys_set_options(void){
    int input_e;
    int input_f;
    int input_s;
    int input_o;

    argint(0, &input_e);
    argint(1, &input_f);
    argint(2, &input_s);
    argint(3, &input_o);
    return set_options(input_e, input_f, input_s, input_o);
}

int sys_get_callcount(void){
    return callcount;
}
