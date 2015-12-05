#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <asm/current.h>
#include <linux/sched.h>


#define MESSAGE_LENGTH 80
#define PROC_ENTRY_FILENAME "rw_test"
static char Message[MESSAGE_LENGTH];
struct proc_dir_entry *Our_Proc_File;
extern struct proc_dir_entry proc_root;
int module_open(struct inode *inode,struct file *file)
{
	try_module_get(THIS_MODULE);
	return 0;
}

int module_close(struct inode *inode,struct file *file)
{
	module_put(THIS_MODULE);
	return 0;
}

static ssize_t module_input(struct file *filp,const char * buff,size_t len,loff_t *off)
{
	int i;
	for(i=0 ; i < MESSAGE_LENGTH-1 && i < len ;i++)
		get_user(Message[i],buff+i);

	Message[i] = '\0';
	return i;
}

static ssize_t module_output(struct file *filp,char *buffer ,size_t length ,loff_t * offset)
{
	static int finished = 0;
	int i;
	char message[MESSAGE_LENGTH + 30];
	if(finished){
		finished = 0;
		return 0;
	}
	sprintf(message,"Last input:%s.",Message);
	for(i=0 ; i < length && message[i] ; i++)
		put_user(message[i],buffer + i);

	finished = 1;
	return i;
}

static int module_permission(struct inode *inode,int op)
{
	if(current->real_cred->uid == 0)
		return 0;
	return -EACCES;
}

static struct file_operations File_Ops_4_Our_Proc_File={
	.read = module_output,
	.write = module_input,
	.open = module_open,
	.release = module_close,
};


static struct inode_operations Inode_Ops_4_Our_Proc_File={
	.permission = module_permission,
};

int init_module(void)
{
	int rv = 0;
	Our_Proc_File = create_proc_entry(PROC_ENTRY_FILENAME,0644,NULL);
	//Our_Proc_File->read_proc = procfile_read;
	Our_Proc_File->mode = S_IFREG |S_IRUGO;
	Our_Proc_File->uid = 0;
	Our_Proc_File->gid = 0;
	Our_Proc_File->size = 80;
	//normal VFS interface format;
	Our_Proc_File->proc_iops = &Inode_Ops_4_Our_Proc_File;
	Our_Proc_File->proc_fops = &File_Ops_4_Our_Proc_File;
	printk(KERN_INFO "Trying to create /proc/rw_test:\n");
	if(Our_Proc_File == NULL){
		rv = -ENOMEM;
		remove_proc_entry(PROC_ENTRY_FILENAME,NULL);
		printk(KERN_INFO "Error:Could not initialize /proc/rw_test.\n");
	}else{
		printk(KERN_INFO "Success!\n");
	}
	return rv;
}

void cleanup_module(void)
{
	remove_proc_entry(PROC_ENTRY_FILENAME,NULL);
	printk(KERN_INFO "/proc/rw_test has been removed.\n");
}
