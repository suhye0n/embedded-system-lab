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

static ssize_t clcd_write(struct file* filp, const char* buffer, size_t length, loff_t* offset)
{
	int i;
	char str[100];
	copy_from_user(str, buffer, length);
	str[length] = '\0';
	printk("[Drv] %s\n", str);
	//clcd_out(E,RW,RS,data);
	clcd_out(1, 0, 0, 0x02);
	for (i = 0; i < length && i < 32 && str[i] != '\0'; i++)
	{
		if (str[i] == '\n')
		{
			clcd_out(1, 0, 0, 0xC0); // 0xC0 (1100 0000)
		}
		else
		{
			clcd_out(1, 0, 1, str[i]);
		}
	}
	return 0;
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
	open: clcd_open,
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
	clcd_out(1, 0, 0, 0x38); // 0x38 : 0011 1000
	clcd_out(1, 0, 0, 0x0C); // 0x0C : 0000 1100
	clcd_out(1, 0, 0, 0x01); // 0x01 : 0000 0001
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
