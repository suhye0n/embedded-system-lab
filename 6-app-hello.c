#include <unistd.h>
#include <fcntl.h>

int main(int argc, char** argv)
{
  int fd, nread;
  char buffer[1024];
  if (argc > 1)
    strcpy(buffer, argv[1]);
  else
    strcpy(buffer, "application message");
  
  fd = open("/dev/hello", O_RDWR);
  write(fd, buffer, strlen(buffer));
  nread = read(fd, buffer, 1024);
  buffer[nread] = '\0';
  printf("%s\n", buffer);
  
  close(fd);
  return 0;
}
