#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/fs.h>
#include <linux/file.h>
#include <linux/fs_struct.h>
#include <linux/fdtable.h>
#include <linux/string.h>
#include <linux/mm.h>
#include <linux/syscalls.h>
#include <linux/list.h>
#include <linux/jiffies.h>
#include <linux/cdev.h>
#include <asm/unistd.h>
#include <asm/uaccess.h>
#include <linux/path.h>
#include <linux/time.h>
#include <linux/stat.h>
#include <net/sock.h>
#include <net/inet_sock.h>
#include <asm/cpufeature.h>

unsigned long **sys_call_table = (unsigned long **)0xffffffff81600560;
unsigned long *orig_mkdir = NULL;

/*make the page writable*/
int make_rw(unsigned long address)
{
	unsigned int level;
	pte_t *pte = lookup_address(address,&level);
	if(pte->pte & ~_PAGE_RW)
		pte->pte |= _PAGE_RW;

	return 0;
}
/*make the page write protected*/
int make_ro(unsigned long address)
{
	unsigned int level;
	pte_t *pte = lookup_address(address,&level);
	pte->pte &= ~_PAGE_RW;

	return 0;
}

asmlinkage long hacked_mkdir(const char __user *pathname,int mode)
{
	printk("mkdir pathname:%s\n",pathname);
	printk(KERN_ALERT "mkdir do nothing!\n");

	return 0;/*everything is ok, but the new systemcall does nothing*/
}

static int syscall_init_module(void)
{
	printk(KERN_ALERT "sys_call_table:0x%p\n",sys_call_table);
	orig_mkdir = (unsigned long *)(sys_call_table[__NR_mkdir]);
	printk(KERN_ALERT "orig_mkdir:0x%p\n",orig_mkdir);

	make_rw((unsigned long )sys_call_table);
	sys_call_table[__NR_mkdir] = (unsigned long *)hacked_mkdir;
	make_ro((unsigned long)sys_call_table);

	return 0;
}

static void syscall_cleanup_module(void)
{
	printk(KERN_ALERT "Module syscall unloaded.\n");

	make_rw((unsigned long )sys_call_table);
	sys_call_table[__NR_mkdir] = (unsigned long *)orig_mkdir;
	/*set mkdir syscall to the origal one*/
	make_ro((unsigned long)sys_call_table);
}

module_init(syscall_init_module);
module_exit(syscall_cleanup_module);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("hack syscall");
