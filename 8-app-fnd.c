#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

int main() {

	int fd;
	int value;

	fd = open("/dev/fnd", O_RDWR);

	scanf("%d", &value); 

	printf("20181090 20200461 \n");
	write(fd, &value, sizeof(int));

	close(fd);
	return 0;
}
