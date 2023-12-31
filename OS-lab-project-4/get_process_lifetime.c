#include "types.h"
#include "user.h"


int main(int argc, char *argv[])
{
    int forkpid = fork();
    if (forkpid == 0){
        sleep(1000);
        printf(1, "child lifetime:%d\n", get_process_lifetime(getpid()) / 100);
    }else{
        wait();
        sleep(200);
        printf(1, "parent lifetime:%d\n", get_process_lifetime(getpid()) / 100);
    }
    exit();
}