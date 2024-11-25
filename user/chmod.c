#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  if(argc != 3){
    printf("Usage: chmod <permissions> <filename>\n");
    exit(1); // Error: incorrect number of arguments
  }

  int perm = atoi(argv[1]);
  if(chmod(argv[2], perm) < 0){
    printf("chmod: failed to change permissions\n");
    exit(1); // Error: chmod failed
  }

  exit(0); // Success: chmod completed successfully
}