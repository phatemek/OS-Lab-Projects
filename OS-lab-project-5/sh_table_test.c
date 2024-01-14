#include "types.h"
#include "user.h"

int main(){
    open_sharedmem(1);
    write_sh_table(0);
    int pid = fork();
    if (pid == 0){
        open_sharedmem(1);
        exit();
    }
    for (int i = 1; i < 5; i++){
        int pid = fork();
       if (pid == 0){
            open_sharedmem(1);
            write_sh_table(i);
            close_sharedmem(1);
           exit();
        }
    }
    while (wait() != -1);
    int res = write_sh_table(1);
    close_sharedmem(1);
    printf(1, "the result for 5 pid is %d.\n", res);
    exit();
}