#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/ioport.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/ioport.h>
#include <unistd.h>
#include <linux/slab.h>
#include <linux/mm.h>
#include <asm/hardware.h>
#include <asm/uaccess.h>
#include <asm/irq.h>
#define FND_MAJOR 65
#define BASE_ADDR 0xF3000000 // 베이스 주소-보드에 따라 다름
#define SEG1_OFFSET 0x0600
#define SEG2_OFFSET 0x0700
#define SEG3_OFFSET 0x0800
#define SEG4_OFFSET 0x0900
#define SEG5_OFFSET 0x0A00
#define SEG6_OFFSET 0x0B00

unsigned short fnd_offset[6] = { SEG1_OFFSET, SEG2_OFFSET, SEG3_OFFSET, SEG4_OFFSET, SEG5_OFFSET, SEG6_OFFSET };
// '0' ~ 'F' 의 폰트
unsigned char num[16] = { 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x27, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71 };
unsigned char alpha_font[26] = { // 임의로 만들어본 알파벳 폰트
 0x77,0x7c,0x58,0x5e,0x7b,0x71,0x6f,0x76,0x06,0x0e,0x75,0x38,0x79,
 0x54,0x3f,0x73,0x67,0x50,0x6d,0x78,0x3e,0x72,0x4f,0x49,0x6e,0x5b
};

int fnd_open(struct inode* inode, struct file* filp)
	{
	//MOD_INC_USE_COUNT;
	return 0;
}

int fnd_release(struct inode* inode, struct file* filp)
{
	//MOD_DEC_USE_COUNT;
	return 0;
}

static ssize_t fnd_write(struct file* file, const char* buffer, size_t length, loff_t* offset)
{
	int i;
	unsigned char* fnd_base, * fnd_addr;
	int value, tmp;

	get_user(value, (int*)buffer);

	printk("[Drv] write : %d\n", value);

	fnd_base = (unsigned char*)(BASE_ADDR);


	for (i = 5; i >= 0; i--) {

		if(value == 0) break;

		tmp = value % 10;
		value /= 10;

		fnd_addr = (unsigned char*)(fnd_base + fnd_offset[i]);
		*fnd_addr = num[tmp];
	}

	for(i; i >=0; i--){
		
		fnd_addr = (unsigned char*)(fnd_base + fnd_offset[i]);
		*fnd_addr = 0;
	}

	return length;
}

struct file_operations fnd_fops = {
	open : fnd_open,
	write : fnd_write,
	release : fnd_release,
};

static int __init fnd_init(void)
{
	int result;
	if ((result = register_chrdev(FND_MAJOR, "FND device", &fnd_fops)) < 0)
	{
		printk("FND-init_module: can't get major %d.\n", FND_MAJOR);
		return result;
	}

	printk("FND-init_module: major number is %d.\n", FND_MAJOR);
	return 0;
}

static void __exit fnd_exit(void)
{
	if ((unregister_chrdev(FND_MAJOR, "FND device")) == 0)
		printk(KERN_WARNING "FND-cleanup_module: succeeded.. bye\n");
	else
		printk(KERN_WARNING "FND-cleanup_module: failed\n");
}

module_init(fnd_init);
module_exit(fnd_exit);
