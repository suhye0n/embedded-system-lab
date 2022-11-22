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
#define HELLO_MAJOR 200

static int hello_open(struct inode* inode, struct file* fp)
{
  //MOD_INC_USE_COUNT;
  return 0;
}

static int hello_release(struct inode* inode, struct file* fp)
{
  //MOD_DEC_USE_COUNT;
  return 0;
}

static ssize_t hello_write(struct file* fp, const char* buffer, size_t bufsize, loff_t* len)
{
  int i, length;
  char knl_buf[1024];
  copy_from_user(knl_buf, buffer, bufsize);
  knl_buf[bufsize] = '\0';
  length = strlen(knl_buf);
  i = length + 2;
  while (i--) printk("*");
  printk("\n*%s*\n", knl_buf);
  printk("20200461 박수현\n"); // 학번 출력
  printk("20200380 나현진\n"); // 학번 출력
  i = length + 2;
  while (i--) printk("*");
  printk("\n");
  return bufsize;
}

static ssize_t hello_read(struct file* fp, char* buffer, size_t bufsize, loff_t* len)
{
  char knl_msg[] = "from driver";
  //put_user(knl_msg, buffer);
  copy_to_user(buffer, knl_msg, strlen(knl_msg));
  return strlen(knl_msg);
}

static struct file_operations hello_fops = {
  write: hello_write,
    read : hello_read,
  open : hello_open,
  release : hello_release,
};

int init_module(void)
{
  int result;
  printk("Hello is loading...!\n");
  if ((result = register_chrdev(HELLO_MAJOR, "Hello device", &hello_fops)) < 0)
  {
    printk("HELLO-init_module: can't get major %d.\n", HELLO_MAJOR);
    return result;
  }
  printk("HELLO-init_module: major number is %d.\n", HELLO_MAJOR);
  return 0;
}

void cleanup_module(void)
{
  if ((unregister_chrdev(HELLO_MAJOR, "Hello device")) == 0)
  printk(KERN_WARNING "HELLO-cleanup_module: succeeded.. bye\n");
  else
  printk(KERN_WARNING "HELLO-cleanup_module: failed\n");
}
