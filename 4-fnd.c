#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>

#define CS3_BASE 0x0C000000

unsigned short FND_OFFSET[6] = { 0x0600, 0x0700, 0x0800, 0x0900, 0x0A00, 0x0B00 };
// 이것은 일종의 폰트 데이터

unsigned char FND_FONT[10] = { 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x27, 0x7F, 0x6F };
// '0' '1' '2' '3' '4' '5' '6' '7' '8' '9'


unsigned char* CS3_base;
unsigned char* fnd;
int fd;


int main(void)
{

	printf("20180671 안태욱\n");
	printf("20200461 박수현\n");
		

	// 1. 메모리 장치 파일 열기
	fd = open("/dev/mem", O_RDWR);
	if (fd < 0)
	{
		perror("/dev/mem FAIL! \n");
		exit(1);
	}


	while(1) {

		int input;
		scanf("%d", &input);

		int flag = input;


		// 2. mmap 함수 호출
		CS3_base = (unsigned char*)mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, CS3_BASE);
		if ((int)CS3_base == -1)
		{
			perror("mmap FAIL! \n");
			return -1;
		}


		// 3. 장치로 출력
		int index = 5;

		for(index; index >= 0; index--) {

			if(input == 0) break;


			int tmp = input % 10;
			input /= 10;

			fnd = (unsigned char*)(CS3_base + FND_OFFSET[index]);
			*fnd = FND_FONT[tmp];
		
		}

		for(index; index >= 0; index--) {

			fnd = (unsigned char*)(CS3_base + FND_OFFSET[index]);
			*fnd = 0;

		}


		// 4. munmap 함수 호출
		munmap((unsigned char*)CS3_base, 4096);
		


		if(flag == 0) break;


	}



	// 메모리 장치 파일 닫기
	close(fd);
	return 0;

}
