#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define SUCCESS 0
#define DEVICE_NAME "chardev"
#define BUF_LEN 80

static int Major;
static char msg[BUF_LEN];
static char *msg_Ptr;

static int Device_Open = 0;
static int device_open(struct inode *inode,struct file *file)
{
	static int counter = 0;
	if(Device_Open)
		return -EBUSY;
	Device_Open++;
	sprintf(msg,"I already told you %d times Hello world.\n",counter++);
	msg_Ptr = msg;
	try_module_get(THIS_MODULE);
	return SUCCESS;
}

static int device_release(struct inode *inode,struct file *file)
{
	Device_Open--;
	module_put(THIS_MODULE);
	return 0;
}

static ssize_t device_read(struct file *file,char *buffer,size_t length,loff_t *offset)
{
	if(*msg_Ptr == 0)
		return 0;
	return copy_to_user(buffer,msg_Ptr,length);
}

static ssize_t device_write(struct file *file,const char *buff,size_t len,loff_t *off)
{
	printk("<1> Sorry this operation isn't supported.\n'");
	return -EINVAL;
}

static struct file_operations fops={
	.read = device_read,
	.write = device_write,
	.open = device_open,
	.release = device_release
};

int init_module(void)
{
	Major = register_chrdev(0,DEVICE_NAME,&fops);
	if(Major < 0){
		printk("Registering the character device failed with %d.\n",Major);
		return Major;
	}
	printk("<1> I was assigned major number %d",Major);
	printk("<1> the drive,create a dev file");
	printk("<1> mknod /dev/hello c %d 0.\n",Major);	
	printk("<1> I was assigned major number %d",Major);
	printk("<1> the device file \n");
	printk("<1> Remove the file device and module when done\n");
	return 0;
}

void cleanup_module(void)
{
	unregister_chrdev(Major,DEVICE_NAME);
}
