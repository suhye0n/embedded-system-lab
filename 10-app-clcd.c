#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <linux/ioctl.h>

#define LCD_DEV_MAGIC 'Y'
#define LCD_INIT _IO(LCD_DEV_MAGIC, 0)
#define LCD_SET_POS _IOW(LCD_DEV_MAGIC, 1, unsigned char) // 추가적 값 전달 
// 디바이스 드라이버랑 똑같이 추가 -> 그래야 정확하게 동작 


int main()
{
	int fd;
	fd = open("/dev/clcd", O_RDWR);
	printf("input string (max 32 chars) : ");
	char *msg1 = "20190610";
	char *msg2 = "20200461";

	ioctl(fd, LCD_INIT); // 명령어 하나뿐 -> 나중에 더 추가
	ioctl(fd, LCD_SET_POS, 17); 
	write(fd, msg1, strlen(msg1));
	
	ioctl(fd, LCD_SET_POS, 1); // 아이오더블유 사용 (뭐,뭐,캐릭터) 12페이지 
	write(fd, msg2, strlen(msg2));

	close(fd);
	return 0;
}
