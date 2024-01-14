#include "plock.h"


void
initplock(struct plock* pl, char* name) {
    initlock(&pl->lk, "sleep lock");
    pl->name = name;
    pl->locked = 0;
    pl->lock_holder = 0;
    pl->waiting_cnt = 0;
    memset(pl->queue, 0, sizeof(pl->queue));
}

int
is_free(struct plock* pl) {
    return (pl->waiting_cnt == 0 && !pl->locked);
}

int
contains(struct plock* pl, int pid) {
    for (int i = 0; i < NPROC; i++) {
        if (pl->queue[i] == pid) {
            return 1;
        }
    }
    return 0;
}

void
enqueue(struct plock* pl, int pid) {
    pl->waiting_cnt++;
    int idx = 0;
    while (pl->queue[idx] > pid) {
        idx++;
    }
    for (int i = NPROC-1; i > idx; i--) {
        pl->queue[i] = pl->queue[i-1];
    }
    pl->queue[idx] = pid;
}

int
is_first(struct plock* pl, int pid) {
    return (pl->queue[0] == pid);
}

void
dequeue(struct plock* pl, int pid) {
    pl->waiting_cnt--;
    int idx;
    for (int i = 0; i < NPROC; i++) {
        if (pl->queue[i] == pid) {
            idx = i;
            break;
        }
        if (i == NPROC - 1) return;
    }
    for(int i = idx; i < NPROC-1; i++) {
        pl->queue[i] = pl->queue[i+1];
    }
    pl->queue[NPROC-1] = 0;
}

void
acquireplock(struct plock* pl) {
    acquire(&pl->lk);
    cprintf("pid no %d joined\n", myproc()->pid);
    if (is_free(pl)) {
        pl->locked = 1;
        pl->lock_holder = myproc()->pid;
        cprintf("pid no %d aquired the lock\n", myproc()->pid);
    }
    else {
        if (!contains(pl, myproc()->pid)) {
            enqueue(pl, myproc()->pid);
        }
        while (pl->locked || !is_first(pl, myproc()->pid)) {
            sleep(pl, &pl->lk);
        }
        cprintf("pid no %d aquired the lock\n", myproc()->pid);
        pl->locked = 1;
        pl->lock_holder = myproc()->pid;
        dequeue(pl, myproc()->pid);
    }
    release(&pl->lk);
}

void
releaseplock(struct plock* pl) {
    if (pl->lock_holder == myproc()->pid) {
        acquire(&pl->lk);
        cprintf("pid no %d released the lock\n", myproc()->pid);
        pl->locked = 0;
        pl->lock_holder = 0;
        wakeup(pl);
        release(&pl->lk);
    }
}