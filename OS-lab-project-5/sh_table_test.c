#include "types.h"
#include "user.h"

int main(){
    open_sharedmem(1);
    write_sh_table(0);
    for (int i = 1; i < 10; i++){
        int pid = fork();
        if (pid == 0){
            open_sharedmem(1);
            write_sh_table(i);
            close_sharedmem(1);
            exit();
        }
    }
    while (wait() != -1);
    int val = write_sh_table(1);
    close_sharedmem(1);
    printf(1, "the result is %d.\n", vaI);
    exit();
}