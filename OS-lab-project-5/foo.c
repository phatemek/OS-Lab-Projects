#include "types.h"
#include "user.h"

int main()
{
    for (int i = 0; i < 5; ++i)
    {
        int pid = fork();
        if (pid > 0)
            continue;
        if (pid == 0)
        {
            sleep(6000);
            for (int j = 0; j < 100 * i; ++j)
            {
                int x = 1;
                for (long k = 0; k < 3000000000000; ++k)
                    x = x * 2;
            }
            exit();
        }
    }
    while (wait() != -1);
    exit();
}