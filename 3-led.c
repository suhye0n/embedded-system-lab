#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#define CS3_BASE 0x0C000000
#define LED_OFFSET 0x0C00

int main(void)
{
   int fd;
   unsigned char* CS3_base;
   unsigned char* led;
   
	printf("20180671 안태욱\n");
	printf("20200461 박수현\n");
  
   /* 1: 메모리 장치 파일 열기 */
   fd = open("/dev/mem", O_RDWR);
   if (fd < 0)
   {
     perror("/dev/mem FAIL! \n");
     exit(1);
   }
   
   while(1) {
		int input;
		scanf("%d", &input);
      
      /* 2: mmap 함수 호출 */
      CS3_base = (unsigned char*)mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, CS3_BASE);
      if ((int)CS3_base == -1)
      {
        perror("mmap FAIL! \n");
        return -1;
      }

      /* 3: LED 포트에 값 쓰기 */
      led = CS3_base + LED_OFFSET;
      *led = input;

      /* 4: munmap 함수 호출 */
      munmap((unsigned char*)CS3_base, 4096);
   }
  
   /* 5: 메모리 장치 파일 닫기 */
   *led = 0
   close(fd);
   return 0;
}
