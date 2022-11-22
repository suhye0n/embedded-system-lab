#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/ioport.h>
#include <linux/slab.h>
#include <linux/mm.h>
#include <asm/hardware.h>
#include <asm/uaccess.h>
#include <asm/irq.h>
#include <unistd.h>
#define LED_MAJOR 64
#define LED_ADDR 0xF3000C00

static int led_open(struct inode* inode, struct file* fp)
{
  //MOD_INC_USE_COUNT;
  return 0;
}

static int led_release(struct inode* inode, struct file* fp)
{
  //MOD_DEC_USE_COUNT;
  return 0;
}

static ssize_t led_write(struct file* fp, const char* buffer, size_t bufsize, loff_t* len)
{
  int i;
  unsigned char* led_addr = (unsigned char*)LED_ADDR;
  get_user(i, (int*)buffer);
  printk("[Driver] get value : %d\n", i);
  *led_addr = i;
  return bufsize;
}

static struct file_operations led_fops = {
  write : led_write,
  open : led_open,
  release : led_release,
};

static int __init led_init(void)
{
  int result;
  if ((result = register_chrdev(LED_MAJOR, "LED device", &led_fops)) < 0)
  {
    printk("LED-init_module: can't get major %d.\n", LED_MAJOR);
    return result;
  }
  printk("LED-init_module: major number is %d.\n", LED_MAJOR);
  return 0;
}

static void __exit led_exit(void)
{
  if ((unregister_chrdev(LED_MAJOR, "LED device")) == 0)
    printk(KERN_WARNING "LED-cleanup_module: succeeded.. bye\n");
  else
    printk(KERN_WARNING "LED-cleanup_module: failed\n");
}

module_init(led_init);
module_exit(led_exit);
