#include <stdio.h>
#include <syscall.h>

#define SIZE 0x100000

int main(void)
{
  printf("create 1M file\n");
  if(!create("file", SIZE))
  {
    printf("failed!\n");
    return -1;
  }
  else
  {
    printf("success!\n");
    return 0;
  }
}
