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
#define CLCD_ADDR 0xF3000300 // base address + offset
static int CLCD_MAJOR = 66;
unsigned short* clcd;


// 추가된 부분 : 매직 넘버와 명령어 번호
#define LCD_DEV_MAGIC 'Y'
#define LCD_INIT _IO(LCD_DEV_MAGIC, 0)
#define LCD_SET_POS _IOW(LCD_DEV_MAGIC, 1, unsigned char) // 추가적 값 전달 
//

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

void set_pose(arg)
{
	if (arg >= 1 && arg <= 16) {
		arg -= 1;
	} else if (arg >= 17 && arg <= 32) {
		arg = 0x40 + (arg-17);
	}
	clcd_out(1, 0, 0, 0x80+arg);
}

// 추가된 부분 : 사용자의 ioctl 호출에 대응하는 함수 (아이오컨트롤 -> 쓰기 함수 간편해짐)
int clcd_ioctl(struct inode* inode, struct file* filp, unsigned int cmd, unsigned long arg) // 응용에서 ioctl, 씨엠디: 엘씨디이닛 담김, 초기화하려면 아이오컨트롤 호출 
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

		// 새로운 ioctl 명령어는 이쯤에 추가한다.
		case LCD_SET_POS:
			set_pose(arg);
			 // 안에 어떻게 동작할지는 만드는 사람 마음
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
	// 아이오컨트롤에서 커서 위치 바꾸는 명령어.....
	int i;
	char str[100];
	copy_from_user(str, buffer, length);
	str[length] = '\0';
	printk("[Drv] %s\n", str);

	for (i = 0; i < 16 && str[i] != '\0'; i++)
		clcd_out(1, 0, 1, str[i]);

	return length; 
}

int clcd_open(struct inode* inode, struct file* filp) // 지난주와 동일
{
	printk("open()\n");
	//MOD_INC_USE_COUNT;
	return 0;
}

int clcd_release(struct inode* inode, struct file* filp) // 지난주와 동일
{
	printk("close()\n");
	//MOD_DEC_USE_COUNT;
	return 0;
}

static struct file_operations clcd_fops = {
ioctl: clcd_ioctl, // 추가됨
open : clcd_open,
write : clcd_write,
release : clcd_release,
};

static int __init clcd_init(void) // !! 실행하면 여기 호출됨 
{
	int result;
	if ((result = register_chrdev(CLCD_MAJOR, "CLCD device", &clcd_fops)) < 0)
	{
		printk("CLCD-init_module: can't get major %d.\n", CLCD_MAJOR);
		return result;
	}
	printk("CLCD-init_module: major number is %d.\n", CLCD_MAJOR);
	clcd = (unsigned short*)CLCD_ADDR;
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
