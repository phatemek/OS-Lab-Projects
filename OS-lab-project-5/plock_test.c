#include "types.h"
#include "user.h"

int main()
{
  printf(1, "Running plock test\n");
  plock_init();
  for (int i = 0; i < 10; i++)
  {
    int pid = fork();
    if (pid < 0)
    {
      printf(1, "Fork failed.\n");
      exit();
    }
    else if (pid == 0)
    {
      plock_aquire();
      sleep(100);
      plock_release();
      exit();
    }
  }
  while (wait() != -1);
  exit();
}