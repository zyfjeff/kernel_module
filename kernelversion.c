#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/utsname.h>

#define AUTHOR "zyf"
#define DESC_THIS "A simple get kernel version"

static int __init init_version(void)
{
	struct new_utsname *name = &(init_uts_ns.name);
	printk(KERN_INFO "Hello,World 4\n");
	printk("kernel sysname:%s\nkernel nodename:%s\nkernel release:%s\nkernel version:%s\nkernel machine:%s\n",
			name->sysname,name->nodename,name->release,name->version,name->machine);	
	return 0;
}

static void __exit cleanup_version(void)
{
	printk(KERN_INFO "Goodbye,word 4\n");
	return;
}

module_init(init_version);
module_exit(cleanup_version);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(AUTHOR);
MODULE_DESCRIPTION(DESC_THIS);
