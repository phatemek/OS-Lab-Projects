#include "types.h"
#include "defs.h"
#include "param.h"
#include "x86.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "spinlock.h"

struct plock {
    int locked;
    struct spinlock lk;
    char* name;
    int lock_holder;
    int queue[NPROC];
    int waiting_cnt;
}myplock;