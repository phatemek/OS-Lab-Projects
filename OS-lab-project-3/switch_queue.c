#include "types.h"
#include "user.h"

void set_queue(int pid, int queue)
{
    if (pid < 1)
    {
        printf(1, "Invalid pid\n");
        return;
    }
    if (queue < 1 || queue > 3)
    {
        printf(1, "Invalid queue\n");
        return;
    }
    int res = switch_queue(pid, queue);
    if (res < 0)
        printf(1, "Error changing queue\n");
    else
        printf(1, "Queue changed successfully\n");
}

int main(int argc, char* argv[]){
    if (argc < 3){
        exit();
    }
    set_queue(atoi(argv[1]), atoi(argv[2]));
    exit();
}