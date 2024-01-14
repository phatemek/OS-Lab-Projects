#include "types.h"
#include "user.h"

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

int main(int argc, char* argv[]){
    if (argc < 4)
        exit();
    set_system_bjf(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]), atoi(argv[4]));
    exit();
}