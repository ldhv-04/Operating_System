#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
  if(argc < 3) {
    fprintf(2, "Usage: trace mask command [args...]\n");
    exit(1);
  }

  int mask = atoi(argv[1]);
  trace(mask);  // Call the sys_trace system call

  exec(argv[2], &argv[2]);  // Execute the command to trace
  exit(0);
}
