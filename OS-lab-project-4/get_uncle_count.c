#include "types.h"
#include "user.h"

void child4(){
    int forkpid = fork();
    if (forkpid > 0)
        wait();
    else if (forkpid == 0)
        printf(1, "number of process %d uncles: %d\n", getpid(), get_uncle_count(getpid()));
    else
        printf(2, "child4 fork failed.\n");
    exit();
}

void child3(){
    int forkpid3 = fork();
    if (forkpid3 < 0){
        printf(2, "Third child fork failed.\n");
        exit();
    }
    else if (forkpid3 == 0)
        child4();
}

void child2(){
    int forkpid2 = fork();
    if (forkpid2 < 0){
        printf(2, "Second child fork failed\n");
        exit();
    }
    if (forkpid2 == 0)
        sleep(50);
    else
        child3();
    while (wait() != -1);
}

int main(int argc, char *argv[])
{
    int forkpid1 = fork();
    if (forkpid1 < 0){
        printf(2, "First child fork failed\n");
        exit();
    }
    else if (forkpid1 == 0){
        sleep(50);
    }
    else{
        child2();
    }
    exit();
}