// init: The initial user-level program

#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/spinlock.h"
#include "kernel/sleeplock.h"
#include "kernel/fs.h"
#include "kernel/file.h"
#include "user/user.h"
#include "kernel/fcntl.h"

char *argv[] = { "sh", 0 };

int main(void) {
  int pid, wpid;

  printf("init: starting\n");

  if(open("console", O_RDWR) < 0){
    printf("init: open console failed, creating console\n");
    mknod("console", CONSOLE, 0);
    open("console", O_RDWR);
  } else {
    printf("init: console opened successfully\n");
  }
  dup(0);  // stdout
  dup(0);  // stderr

  for(;;){
    printf("init: starting sh\n");
    pid = fork();
    if(pid < 0){
      printf("init: fork failed\n");
      exit(1);
    }
    if(pid == 0){
      printf("init: exec sh\n");
      exec("sh", argv);
      printf("init: exec sh failed\n");
      exit(1);
    }
    for(;;){
      printf("init: waiting for sh to exit\n");
      wpid = wait((int *) 0);
      if(wpid == pid){
        printf("init: sh exited, restarting\n");
        break;
      } else if(wpid < 0){
        printf("init: wait returned an error\n");
        exit(1);
      } else {
        printf("init: reaped zombie process\n");
      }
    }
  }
}