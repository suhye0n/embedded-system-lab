#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/ioport.h>
#include <linux/slab.h>
#include <linux/mm.h>
#include <asm/hardware.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <asm/delay.h>
#include <unistd.h>
#include "hd.h"
#include <linux/ioctl.h>

#define CLCD_ADDR 0xF3000300 // base address + offset

static int CLCD_MAJOR = 66;
unsigned short* clcd;
void clcd_out(int e, int rw, int rs, unsigned char value)
{
unsigned short result = 0;
result |= e ? 0x0400 : 0; // e=1:enable, e=0:disable
result |= rw ? 0x0200 : 0; // rw=1:read, rw=0:write
result |= rs ? 0x0100 : 0; // rs=1:char, rs=0:instruction
result |= value;
*clcd = result;
udelay(1000);
*clcd = result ^ 0x0400; // 0x40 : 00000100 00000000
udelay(1000);
}
void initialize(void)
{
clcd_out(1, 0, 0, 0x38);
clcd_out(1, 0, 0, 0x0C);
clcd_out(1, 0, 0, 0x01);
}
void set_position(int arg)
{
	if (arg >= 1 && arg <= 16)
		arg = arg - 1;
	else if (arg >= 17 && arg <= 32)
		arg = 0x40 + (arg - 17);
	else
		arg = 0;
	clcd_out(1,0,0,0x80|(unsigned char)arg); // 1000 0000
	int i = 0;
	for(i; i<16; i++)
		clcd_out(1,0,1,' ');
	clcd_out(1,0,0,0x80|(unsigned char)arg); // 1000 0000
}
// 추가된 부분 : 사용자의 ioctl 호출에 대응하는 함수
int clcd_ioctl(struct inode* inode, struct file* filp, unsigned int cmd, unsigned long arg)
{
if (_IOC_TYPE(cmd) != LCD_DEV_MAGIC)
{
printk("ioctl : command type mismatch\n");
return -1;
}
switch (cmd)
{
case LCD_INIT:
	printk("ioctl : LCD_INIT\n");
	initialize();
break;
// 새로운 ioctl 명령어를 추가한다면 이쯤에 필요한 만큼 case 문을 추가해야 한다.
case LCD_SET_POS:
	printk("ioctl : LCD_SET_POS\n");
	set_position(arg);
	break;
default:
printk("ioctl : unknown command\n");
break;
}
return 0;
}
////////////////////////////////////////////////
// 수정됨 : ioctl을 지원하기 때문에 write는 최소한의 출력 기능만 담당한다.
static ssize_t clcd_write(struct file* filp, const char* buffer, size_t length, loff_t* offset)
{
int i;
char str[100];
copy_from_user(str, buffer, length);
str[length] = '\0';
printk("[Drv] %s\n", str);
for (i = 0; i < 16 && str[i] != '\0'; i++)
clcd_out(1, 0, 1, str[i]);
return length;
}
int clcd_open(struct inode* inode, struct file* filp)
{
printk("open()\n");
//MOD_INC_USE_COUNT;
return 0;
}
int clcd_release(struct inode* inode, struct file* filp)
{
printk("close()\n");
//MOD_DEC_USE_COUNT;
return 0;
}
static struct file_operations clcd_fops = {
ioctl : clcd_ioctl, // 추가됨
open : clcd_open,
write : clcd_write,
release : clcd_release,
};
static int __init clcd_init(void)
{
int result;
if ((result = register_chrdev(CLCD_MAJOR, "CLCD device", &clcd_fops)) < 0)
{
printk("CLCD-init_module: can't get major %d.\n", CLCD_MAJOR);
return result;
}
printk("CLCD-init_module: major number is %d.\n", CLCD_MAJOR);
clcd = (unsigned short*)CLCD_ADDR;
// clcd_out(E,RW,RS,data);
//clcd_out(1, 0, 0, 0x38); // 0x38 : 0011 1000
//clcd_out(1, 0, 0, 0x0C); // 0x0C : 0000 1100
//clcd_out(1, 0, 0, 0x01); // 0x01 : 0000 0001
return 0;
}
static void __exit clcd_exit(void)
{
if ((unregister_chrdev(CLCD_MAJOR, "CLCD device")) == 0)
printk(KERN_WARNING "CLCD-cleanup_module: succeeded.. bye\n");
else
printk(KERN_WARNING "CLCD-cleanup_module: failed\n");
}
module_init(clcd_init);
module_exit(clcd_exit);
