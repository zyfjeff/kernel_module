#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>

#define DRIVER_AUTHOR "zyf"
#define DRIVER_DESC "A sample driver"

static int __init init_hello_4(void)
{
	printk(KERN_INFO "Hello,world 4.\n");
	return 0;
}

static void __exit cleanup_hello_4(void)
{
	printk(KERN_INFO"Goodbye,world 4.\n");
}

module_init(init_hello_4);
module_exit(cleanup_hello_4);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
