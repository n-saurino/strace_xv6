#include "kernel/types.h"
#include "kernel/defs.h"
#include "kernel/param.h"
#include "kernel/memlayout.h"
#include "kernel/mmu.h"
#include "kernel/proc.h"
#include "kernel/x86.h"
#include "kernel/syscall.h"



// OUR FILE
#include "user/strace.h"
#include "kernel/strace.h"
#include "kernel/strace.c"


// User code makes a system call with INT T_SYSCALL.
// System call number in %eax.
// Arguments on the stack, from the user call to the C
// library system call function. The saved user %esp points
// to a saved program counter, and then the first argument.

// Fetch the int at addr from the current process.
int fetchint(uint addr, int *ip) {
  if (addr >= proc->sz || addr + 4 > proc->sz)
    return -1;
  *ip = *(int *)(addr);
  return 0;
}

// Fetch the nul-terminated string at addr from the current process.
// Doesn't actually copy the string - just sets *pp to point at it.
// Returns length of string, not including nul.
int fetchstr(uint addr, char **pp) {
  char *s, *ep;

  if (addr >= proc->sz)
    return -1;
  *pp = (char *)addr;
  ep = (char *)proc->sz;
  for (s = *pp; s < ep; s++)
    if (*s == 0)
      return s - *pp;
  return -1;
}

// Fetch the nth 32-bit system call argument.
int argint(int n, int *ip) { return fetchint(proc->tf->esp + 4 + 4 * n, ip); }

// Fetch the nth word-sized system call argument as a pointer
// to a block of memory of size n bytes.  Check that the pointer
// lies within the process address space.
int argptr(int n, char **pp, int size) {
  int i;

  if (argint(n, &i) < 0)
    return -1;
  if ((uint)i >= proc->sz || (uint)i + size > proc->sz)
    return -1;
  *pp = (char *)i;
  return 0;
}

// Fetch the nth word-sized system call argument as a string pointer.
// Check that the pointer is valid and the string is nul-terminated.
// (There is no shared writable memory, so the string can't change
// between this check and being used by the kernel.)
int argstr(int n, char **pp) {
  int addr;
  if (argint(n, &addr) < 0)
    return -1;
  return fetchstr(addr, pp);
}

extern int sys_chdir(void);
extern int sys_close(void);
extern int sys_dup(void);
extern int sys_exec(void);
extern int sys_exit(void);
extern int sys_fork(void);
extern int sys_fstat(void);
extern int sys_getpid(void);
extern int sys_kill(void);
extern int sys_link(void);
extern int sys_mkdir(void);
extern int sys_mknod(void);
extern int sys_open(void);
extern int sys_pipe(void);
extern int sys_read(void);
extern int sys_sbrk(void);
extern int sys_sleep(void);
extern int sys_unlink(void);
extern int sys_wait(void);
extern int sys_write(void);
extern int sys_uptime(void);
extern int sys_dump(void);
extern int sys_set_call_flag(void);
extern int sys_getN(void);
extern int sys_set_options(void);
extern int sys_strace(void);
int sys_get_callcount(void);


static int (*syscalls[])(void) = {
    [SYS_fork] sys_fork,   [SYS_exit] sys_exit,     [SYS_wait] sys_wait,
    [SYS_pipe] sys_pipe,   [SYS_read] sys_read,     [SYS_kill] sys_kill,
    [SYS_exec] sys_exec,   [SYS_fstat] sys_fstat,   [SYS_chdir] sys_chdir,
    [SYS_dup] sys_dup,     [SYS_getpid] sys_getpid, [SYS_sbrk] sys_sbrk,
    [SYS_sleep] sys_sleep, [SYS_uptime] sys_uptime, [SYS_open] sys_open,
    [SYS_write] sys_write, [SYS_mknod] sys_mknod,   [SYS_unlink] sys_unlink,
    [SYS_link] sys_link,   [SYS_mkdir] sys_mkdir,   [SYS_close] sys_close,
    [SYS_strace]  sys_strace, [SYS_dump]  sys_dump,   [SYS_set_call_flag]  sys_set_call_flag,
    [SYS_getN] sys_getN, [SYS_set_options] sys_set_options, [SYS_get_callcount] sys_get_callcount,
};

static char *system_call_map[] = {
[SYS_fork]    "fork",
[SYS_exit]    "exit",
[SYS_wait]    "wait",
[SYS_pipe]    "pipe",
[SYS_read]    "read",
[SYS_kill]    "kill",
[SYS_exec]    "exec",
[SYS_fstat]   "fstat",
[SYS_chdir]   "chdir",
[SYS_dup]     "dup",
[SYS_getpid]  "getpid",
[SYS_sbrk]    "sbrk",
[SYS_sleep]   "sleep",
[SYS_uptime]  "uptime",
[SYS_open]    "open",
[SYS_write]   "write",
[SYS_mknod]   "mknod",
[SYS_unlink]  "unlink",
[SYS_link]    "link",
[SYS_mkdir]   "mkdir",
[SYS_close]   "close",
[SYS_strace]   "strace",
[SYS_dump]   "dump",
[SYS_set_call_flag]   "set_call_flag",
[SYS_getN]   "getN",
[SYS_set_options]   "set_options",
[SYS_get_callcount] "get_callcount",
};



char* write_trace(char* destination, int pid_input, char* pname_input, char* sysname_input, int returnvalue_input){
    char temp[1000];
    strcat(destination, "TRACE: pid = "); 
    strcat(destination,itoa(pid_input,temp,10));
    memset(&temp[0], 0, sizeof(temp));
    strcat(destination, " | command_name = ");
    strcat(destination, pname_input);
    strcat(destination," | syscall = ");
    strcat(destination, sysname_input);
    strcat(destination," | return value: ");
    strcat(destination,itoa(returnvalue_input,temp,10));
    memset(&temp[0], 0, sizeof(temp));
    return destination;
}

void store_trace(char* trace_message, int callcount){
    int i = 0;
    switch(callcount){
        case 0:
            memset(&trace_1[0], 0, sizeof(trace_1));
            for(i=0; trace_message[i] != 0; i++) {
                trace_1[i] = trace_message[i];
            }
            sysbuf[callcount] = trace_1;
            break;
        case 1:
            memset(&trace_2[0], 0, sizeof(trace_2));
            for(i=0; trace_message[i] != 0; i++) {
                trace_2[i] = trace_message[i];
            }
            sysbuf[callcount] = trace_2;
            break;
        case 2:
            memset(&trace_3[0], 0, sizeof(trace_3));
            for(i=0; trace_message[i] != 0; i++) {
                trace_3[i] = trace_message[i];
            }
            sysbuf[callcount] = trace_3;
            break;
        case 3:
            memset(&trace_4[0], 0, sizeof(trace_4));
            for(i=0; trace_message[i] != 0; i++) {
                trace_4[i] = trace_message[i];
            }
            sysbuf[callcount] = trace_4;
            break;
        case 4:
            memset(&trace_5[0], 0, sizeof(trace_5));
            for(i=0; trace_message[i] != 0; i++) {
                trace_5[i] = trace_message[i];
            }
            sysbuf[callcount] = trace_5;
            break;
        case 5:
            memset(&trace_6[0], 0, sizeof(trace_6));
            for(i=0; trace_message[i] != 0; i++) {
                trace_6[i] = trace_message[i];
            }
            sysbuf[callcount] = trace_6;
            break;
        case 6:
            memset(&trace_7[0], 0, sizeof(trace_7));
            for(i=0; trace_message[i] != 0; i++) {
                trace_7[i] = trace_message[i];
            }
            sysbuf[callcount] = trace_7;
            break;
        case 7:
            memset(&trace_8[0], 0, sizeof(trace_8));
            for(i=0; trace_message[i] != 0; i++) {
                trace_8[i] = trace_message[i];
            }
            sysbuf[callcount] = trace_8;
            break;
        case 8:
            memset(&trace_9[0], 0, sizeof(trace_9));
            for(i=0; trace_message[i] != 0; i++) {
                trace_9[i] = trace_message[i];
            }
            sysbuf[callcount] = trace_9;
            break;
        case 9:
            memset(&trace_10[0], 0, sizeof(trace_10));
            for(i=0; trace_message[i] != 0; i++) {
                trace_10[i] = trace_message[i];
            }
            sysbuf[callcount] = trace_10;
            break;

    }
    
}

void record_syscall(int syscall_id){
    int i;
    struct proc *target_proc = proc;
    char temp_trace[1000];
    memset(&temp_trace[0], 0, sizeof(temp_trace));
    char tproc_name[32];

  // copy command name
    for(i=0; target_proc->name[i] != 0; i++) {
        tproc_name[i] = target_proc->name[i];
    }
    tproc_name[i] = target_proc->name[i];

    if(flag_f != 0 || flag_e != 0 || flag_s != 0){

            if(flag_s != 0){ // flag s is set
              if(target_proc->tf->eax!=-1){
                if(flag_e != 0){ //flag_s and flag_e are set
                    if(syscall_flag == syscall_id){
                        cprintf("\e[1;92mTRACE: pid = %d | command_name = %s | syscall = %s | return value: %d\n\e[0m\n", target_proc->pid, tproc_name, system_call_map[syscall_id], target_proc->tf->eax);
                        
                        // Insert syscall number into ringbuffer
                        if(callcount%N == 0){
                            callcount = 0;
                        }
    
                        write_trace(temp_trace, target_proc->pid, tproc_name, system_call_map[syscall_id], target_proc->tf->eax);

                        store_trace(temp_trace, callcount);
                        
                        callcount++;
                    }
                }else{ // flag_s is set but not flag_e
                    cprintf("\e[1;92mTRACE: pid = %d | command_name = %s | syscall = %s | return value: %d\n\e[0m\n", target_proc->pid, tproc_name, system_call_map[syscall_id], target_proc->tf->eax);
                    
                    // Insert syscall number into ringbuffer
                    if(callcount%N == 0){
                        callcount = 0;
                    }

                    write_trace(temp_trace, target_proc->pid, tproc_name, system_call_map[syscall_id], target_proc->tf->eax);

                    store_trace(temp_trace, callcount);
                        
                    callcount++;
                }
              }
            }else if(flag_f!=0){
              if(target_proc->tf->eax==-1){
                if(flag_e != 0){ //flag_f and flag_e are set
                    if(syscall_flag == syscall_id){
                        cprintf("\e[1;92mTRACE: pid = %d | command_name = %s | syscall = %s | return value: %d\n\e[0m\n", target_proc->pid, tproc_name, system_call_map[syscall_id], target_proc->tf->eax);
                        
                        // Insert syscall number into ringbuffer
                        if(callcount%N == 0){
                            callcount = 0;
                        }

                        write_trace(temp_trace, target_proc->pid, tproc_name, system_call_map[syscall_id], target_proc->tf->eax);

                        store_trace(temp_trace, callcount);
                        
                        callcount++;
                    }
                }else{ // flag_f is set but not flag_e
                    cprintf("\e[1;92mTRACE: pid = %d | command_name = %s | syscall = %s | return value: %d\n\e[0m\n", target_proc->pid, tproc_name, system_call_map[syscall_id], target_proc->tf->eax);

                    // Insert syscall number into ringbuffer
                    if(callcount%N == 0){
                        callcount = 0;
                    }

                    write_trace(temp_trace, target_proc->pid, tproc_name, system_call_map[syscall_id], target_proc->tf->eax);

                    store_trace(temp_trace, callcount);
                        
                    callcount++;
                }
              }    
            }else if(flag_e != 0 && flag_f == 0){ // flag_e is set
                if(syscall_flag == syscall_id){
                    cprintf("\e[1;92mTRACE: pid = %d | command_name = %s | syscall = %s | return value: %d\n\e[0m\n", target_proc->pid, tproc_name, system_call_map[syscall_id], target_proc->tf->eax);
                    
                    // Insert syscall number into ringbuffer
                    if(callcount%N == 0){
                        callcount = 0;
                    }

                    write_trace(temp_trace, target_proc->pid, tproc_name, system_call_map[syscall_id], target_proc->tf->eax);

                    store_trace(temp_trace, callcount);
                    
                    callcount++;                
                }
            }

    }else{
        cprintf("\e[1;92mTRACE: pid = %d | command_name = %s | syscall = %s | return value: %d\n\e[0m\n", target_proc->pid, tproc_name, system_call_map[syscall_id], target_proc->tf->eax);
        
        // Insert syscall number into ringbuffer
        if(callcount%N == 0){
            callcount = 0;
        }

        write_trace(temp_trace, target_proc->pid, tproc_name, system_call_map[syscall_id], target_proc->tf->eax);

        store_trace(temp_trace, callcount);
        
        callcount++;      
    }
}

void syscall(void) {
  int syscall_id;
  struct proc *target_proc = proc;
  int is_traced = (target_proc->traced & TRACE_ON);
  syscall_id = target_proc->tf->eax;

  if(syscall_id == SYS_exit && is_traced) { 
        record_syscall(syscall_id);
    
  }
  if(syscall_id > 0 && syscall_id < NELEM(syscalls) && syscalls[syscall_id]) {
    target_proc->tf->eax = syscalls[syscall_id]();
    if (is_traced) {
        record_syscall(syscall_id);
    }
    
    }else {
        cprintf("%d %s: unknown sys call  \n",
                target_proc->pid, target_proc->name, syscall_id);
        target_proc->tf->eax = -1;
                
  }
}
