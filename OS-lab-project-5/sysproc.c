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
sys_find_digit_root(void){
  cprintf("kernel is running sys_find_digit_root\n");
  return find_digit_root(myproc()->tf->ebx);
}

int 
sys_get_uncle_count(void){
  int pid;
  cprintf("kernel is running sys_get_uncle_count\n");
  argint(0, &pid);
  return get_uncle_count(pid);
}

int 
sys_get_process_lifetime(void){
  cprintf("kernel is running sys_get_process_lifetime\n");
  int pid;
  argint(0, &pid);
  return get_process_lifetime(pid);
}

int
sys_switch_queue(void){
  int pid, queue;
  argint(0, &pid);
  argint(1, &queue);
  return switch_queue(pid, queue);
}

int
sys_bjf_parameters_pl(void) {
  int pid, priority_ratio, arrival_time_ratio, executed_cycle_ratio, process_size_ratio;
  argint(0, &pid);
  argint(1, &priority_ratio);
  argint(2, &arrival_time_ratio);
  argint(3, &executed_cycle_ratio);
  argint(4, &process_size_ratio);
  return bjf_parameters_pl(pid, priority_ratio, arrival_time_ratio, executed_cycle_ratio, process_size_ratio);
}

int
sys_bjf_parameters_sl(void) {
  int priority_ratio, arrival_time_ratio, executed_cycle_ratio, process_size_ratio;
  argint(0, &priority_ratio);
  argint(1, &arrival_time_ratio);
  argint(2, &executed_cycle_ratio);
  argint(3, &process_size_ratio);
  return bjf_parameters_sl(priority_ratio, arrival_time_ratio, executed_cycle_ratio, process_size_ratio);
}

void
sys_show_process_info(void){
  show_process_info();
  return;
}

void 
sys_count_syscalls(void){
  for (int i = 0; i < NCPU; i++){
    cprintf("syscalls from CPU no.%d: %d\n", i, cpus[i].syscall_count);  
  }
  cprintf("total syscall count: %d\n", total_syscall_count);
  return;
}

void
sys_plock_aquire(void){
  plock_aquire();
  return;
}

void
sys_plock_release(void){
  plock_release();
  return;
}

void 
sys_plock_init(void){
  plock_init();
  return;
}

void
sys_open_sharedmem(void) {
  int id;
  argint(0, &id);
  open_sharedmem(id);
  return;
}

void
sys_close_sharedmem(void) {
  int id;
  argint(0, &id);
  close_sharedmem(id);
  return;
}

int
sys_write_sh_table(void){
  int x;
  argint(0, &x);
  write_sh_table(x);
  return;
}