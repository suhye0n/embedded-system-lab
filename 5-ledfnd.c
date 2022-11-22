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
unsigned short FND_OFFSET[6] = { 0x0600, 0x0700, 0x0800, 0x0900, 0x0A00,
0x0B00 };
/* 이것은 일종의 폰트 데이터?? */
unsigned char fnd_font[16] = { 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D,
0x27, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5e, 0x79, 0x71 };
// 순서대로 : 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, A, B, C, D, E, F

int main(void)
{
    int fd;
    int num, fnd;
    unsigned char* CS3_base;
    unsigned char* led_port;
    unsigned char* fnd_port;
    /* 1. 메모리 장치 파일 열기 */
    fd = open("/dev/mem", O_RDWR);
    if (fd < 0)
    {
        perror("/dev/mem FAIL! \n");
        exit(1);
    }

    printf("20200076 권영하\n");
    printf("20200461 박수현\n");

    int input;

    scanf("%d", &input);

    unsigned char arr[13][6] = {
        {0,0,0,0,0,0},
        {0,0,0,0,0,0x06},
        {0,0,0,0,0,0x36},
        {0,0,0,0,0x06,0x36},
        {0,0,0,0,0x36,0x36},
        {0,0,0,0x06,0x36,0x36},
        {0,0,0,0x36,0x36,0x36},
        {0,0,0x06,0x36,0x36,0x36},
        {0,0,0x36,0x36,0x36,0x36},
        {0,0x06,0x36,0x36,0x36,0x36},
        {0,0x36,0x36,0x36,0x36,0x36},
        {0x06,0x36,0x36,0x36,0x36,0x36},
        {0x36,0x36,0x36,0x36,0x36,0x36}
    };

    /* 2. mmap 함수 호출 */
    CS3_base = (unsigned char*)mmap(NULL, 4096, PROT_READ |
    PROT_WRITE, MAP_SHARED, fd, CS3_BASE);
    if ((int)CS3_base == -1)
    {
        perror("mmap FAIL! \n");
        return -1;
    }

    /* 3. LED와 FND로 출력 */
    int tmp = 0x01;
    int sum = 0;
    int value = 8*input/24;
    int i = 0;
    for (i; i<value; i++) {
        sum += tmp;
        tmp *= 2;
    }

    led_port = (unsigned char*)(CS3_base + LED_OFFSET);
    *led_port = sum;

    for (i=0; i<6; i++) {
        fnd_port = (unsigned char*)(CS3_base + FND_OFFSET[i]);
        if (input)
            *fnd_port = arr[input/2][i];
        else *fnd_port = 0;    
    }

    /* 4. munmap 함수 호출 */
    munmap((unsigned char*)CS3_base, 4096);
    usleep(100000); // usleep(1000000) = sleep(1)

    /* 메모리 장치 파일 닫기 */
    close(fd);
    return 0;

}
