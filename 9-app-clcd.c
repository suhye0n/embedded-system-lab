#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
int main()
{
	int fd;
	fd = open("/dev/clcd", O_RDWR);
	char str[] = "20190610\n20200461";
	write(fd, str, strlen(str));
	close(fd);
	return 0;
}
