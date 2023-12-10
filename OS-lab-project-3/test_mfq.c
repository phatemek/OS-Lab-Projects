#include "types.h"
#include "user.h"

void info()
{
    show_process_info();
}

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
    printf(1, "ASFDSAFASFDFD\n");
    int res = switch_queue(pid, queue);
    if (res < 0)
        printf(1, "Error changing queue\n");
    else
        printf(1, "Queue changed successfully\n");
}

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

void set_system_bjf(int priority_ratio, int arrival_time_ratio, int executed_cycle_ratio, int process_size_ratio)
{
    if (priority_ratio < 0 || arrival_time_ratio < 0 || executed_cycle_ratio < 0 || process_size_ratio < 0)
    {
        printf(1, "Invalid ratios\n");
        return;
    }
    bjf_parameters_sl(priority_ratio, arrival_time_ratio, executed_cycle_ratio, process_size_ratio);
    printf(1, "BJF params set successfully\n");
}

int main(int argc, char* argv[])
{
    if (argc < 2){
        exit();
    }
    if (!strcmp(argv[1], "info"))
        info();
    else if (!strcmp(argv[1], "switch_queue"))
    {
        if (argc < 4){
            exit();
        }
        set_queue(atoi(argv[2]), atoi(argv[3]));
    }
    else if (!strcmp(argv[1], "set_process_bjf"))
    {
        if (argc < 6)
            exit();
        
        set_process_bjf(atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), atoi(argv[5]), atoi(argv[6]));
    }
    else if (!strcmp(argv[1], "set_system_bjf"))
    {
        if (argc < 5)
            exit();
        set_system_bjf(atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), atoi(argv[5]));
    }
    exit();
}