#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/ioctl.h>
#include "hd.h"

#define LCD_DEV_MAGIC 'Y'
#define LCD_INIT _IO(LCD_DEV_MAGIC, 0)
#define LCD_SET_POS _IOW(LCD_DEV_MAGIC, 1, int)

int main(void)
{
int fd_key, fd_clcd;
fd_key = open("/dev/key", O_RDWR);
fd_clcd = open("/dev/clcd", O_RDWR);
int value;
char ch;
int pos = 17;
ioctl(fd_clcd, LCD_INIT);
while (1)
{
read(fd_key, &value, sizeof(int));
if (value == 11) // # : exit
break;
else if (value == 10) // * : next line
{
ioctl(fd_clcd, LCD_SET_POS, pos);
pos = (pos == 17) ? 1 : 17;
}
else
{
ch = value + '0';
write(fd_clcd, &ch, sizeof(char));
}
}
close(fd_key);
close(fd_clcd);
return 0;
}
