#include "syscall.h"
#include "traps.h"

#define SYSCALL(name) \
  .globl name; \
  name: \
    movl $SYS_ ## name, %eax; \
    int $T_SYSCALL; \
    ret

SYSCALL(fork)
SYSCALL(exit)
SYSCALL(wait)
SYSCALL(pipe)
SYSCALL(read)
SYSCALL(write)
SYSCALL(close)
SYSCALL(kill)
SYSCALL(exec)
SYSCALL(open)
SYSCALL(mknod)
SYSCALL(unlink)
SYSCALL(fstat)
SYSCALL(link)
SYSCALL(mkdir)
SYSCALL(chdir)
SYSCALL(dup)
SYSCALL(getpid)
SYSCALL(sbrk)
SYSCALL(sleep)
SYSCALL(uptime)
SYSCALL(find_digit_root)
SYSCALL(copy_file)
SYSCALL(get_uncle_count)
SYSCALL(get_process_lifetime)
SYSCALL(switch_queue)
SYSCALL(bjf_parameters_pl)
SYSCALL(bjf_parameters_sl)
SYSCALL(show_process_info)
SYSCALL(count_syscalls)
SYSCALL(plock_aquire)
SYSCALL(plock_release)
SYSCALL(plock_init)
SYSCALL(open_sharedmem)
SYSCALL(close_sharedmem)
SYSCALL(write_sh_table)