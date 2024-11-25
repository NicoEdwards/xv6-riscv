#include <stdio.h>
#include <stdint.h>

#define NDIRECT 12

struct dinode {
  short type;           // File type
  short major;          // Major device number (T_DEVICE only)
  short minor;          // Minor device number (T_DEVICE only)
  short nlink;          // Number of links to inode in file system
  short permissions;    // Campo nuevo de Permisos           // Size of file (bytes)
  unsigned int addrs[NDIRECT+1];   // Data block addresses
};

int main() {
  printf("Size of struct dinode: %lu\n", sizeof(struct dinode));
  return 0;
}