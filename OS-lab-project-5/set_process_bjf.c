#include "types.h"
#include "user.h"

void set_process_bjf(int pid, int priority_ratio, int arrival_time_ratio, int executed_cycle_ratio, int process_size_ratio)
{
    if (pid < 1)
    {
        printf(1, "Invalid pid\n");
        return;
    }
    if (priority_ratio < 0 || arrival_time_ratio < 0 || executed_cycle_ratio < 0)
    {
        printf(1, "Invalid ratios\n");
        return;
    }
    int res = bjf_parameters_pl(pid, priority_ratio, arrival_time_ratio, executed_cycle_ratio, process_size_ratio);
    if (res < 0)
        printf(1, "Error setting BJF params\n");
    else
        printf(1, "BJF params set successfully\n");
}

int main(int argc, char* argv[]){
    if (argc < 5)
        exit();
    set_process_bjf(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), atoi(argv[5]));
    exit();
}