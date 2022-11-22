#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
int main(void)
{
  int fd;
  int key_in = 0;
  fd = open("/dev/key", O_RDWR);
  do
  {
    read(fd, &key_in, 4);
    if(key_in == 10) {
      printf("\n");
    } else if(key_in == 11) {
      break;
    } else {
      printf("%d", key_in);
      fflush(stdout);
    }
  } while (key_in != 11); // '#'

  close(fd);
  return 0;
}
