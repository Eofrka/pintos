#include <syscall.h>
#include <stdio.h>

#define SIZE 0x100000

char buf[SIZE];

int main(void)
{
  int fd = open("file");
  printf("read start!\n");
  read(fd, buf, SIZE);
  printf("finished!\n");

  close(fd);
  return 0;

}
