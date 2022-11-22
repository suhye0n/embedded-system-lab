#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/ioport.h>
#include <linux/slab.h>
#include <linux/mm.h>
#include <asm/hardware.h>
#include <asm/uaccess.h>
#include <asm/irq.h>
#include <unistd.h>
#define KEY_ADDR 0xF3000000
#define KEY_IRQ IRQ_GPIO(19)

static int KEY_MAJOR = 67;
static unsigned short* key;
static int key_data = 0;
static DECLARE_WAIT_QUEUE_HEAD(key_queue);

static int key_open(struct inode* inode, struct file* filp)
{
  //MOD_INC_USE_COUNT;
  return 0;
}

static int key_release(struct inode* inode, struct file* filp)
{
  //MOD_DEC_USE_COUNT;
  return 0;
}

static ssize_t key_read(struct file* filp, char* buf, size_t count, loff_t* l)
{
  interruptible_sleep_on(&key_queue);
  copy_to_user(buf, &key_data, sizeof(key_data));
  return 0;
}

static void key_interrupt(int irq, void* dev_id, struct pt_regs* regs)
{
  key_data = (*(key) & 0xf000) >> 12;
  wake_up_interruptible(&key_queue);
}

static struct file_operations key_fops = {
  open: key_open,
  read : key_read,
  release : key_release,
};

static int __init key_init(void)
{
  int result;
  result = register_chrdev(KEY_MAJOR, "key", &key_fops);
  key = (unsigned short*)KEY_ADDR;
  request_irq(KEY_IRQ, &key_interrupt, SA_INTERRUPT, "key", NULL);
  set_GPIO_IRQ_edge(IRQ_TO_GPIO_2_80(KEY_IRQ), GPIO_FALLING_EDGE);
  printk("install Keypad driver\n");
  return result;
}

void __exit key_exit(void)
{
  disable_irq(KEY_IRQ);
  free_irq(KEY_IRQ, NULL);
  unregister_chrdev(KEY_MAJOR, "key");
  printk("remove Keypad driver\n");
}

module_init(key_init);
module_exit(key_exit);
