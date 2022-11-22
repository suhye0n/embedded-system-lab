#include <unistd.h>
#include <fcntl.h>
int main()
{
  int fd;
  int value;
  fd = open("/dev/led", O_RDWR);
  printf("[App] input value : ");
  scanf("%d", &value);
  write(fd, &value, sizeof(int));
  close(fd);
  return 0;
}
