#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/types.h>

#include "chardev1.h"

#define SUCCESS 0
#define DEVICE_NAME "char_dev"
#define BUF_LEN 80
static int Device_Open = 0;
static int MAJOR_NUM;
static char Message[BUF_LEN];
static char *Message_Ptr;

static int device_open(struct inode *inode,struct file *file)
{
	printk("device_open(%p)\n",file);
	if(Device_Open)
		return -EBUSY;

	Device_Open++;
	Message_Ptr = Message;
	try_module_get(THIS_MODULE);
	return SUCCESS;
}

static int device_release(struct inode *inode,struct file *file)
{
	printk("device_release(%p,%p)\n",inode,file);
	Device_Open--;
	module_put(THIS_MODULE);
	return SUCCESS;
}

static ssize_t device_read(struct file *file ,char __user *buffer, size_t length,loff_t *offset)
{
	int bytes_read = 0;
	printk("device read(%p,%p,%d)\n",file,buffer,length);

	if(*Message_Ptr == 0)
		return 0;
	while(length && *Message_Ptr){
		put_user(*(Message_Ptr++),buffer++);
		length--;
		bytes_read++;
	}
	return bytes_read;
}

static ssize_t device_write(struct file *file,const char __user *buffer,size_t length,loff_t *offset)
{
	int i;
	for(i=0;i < length && i < BUF_LEN;i++)
		get_user(Message[i],buffer+i);

	Message_Ptr = Message;
	return i;
}

int device_ioctl(struct file *file,unsigned int ioctl_num,unsigned long ioctl_param)
{
	int i;
	char *temp;
	char ch;

	switch(ioctl_num){
		case IOCTL_SET_MSG:
			temp = (char *)ioctl_param;
			get_user(ch,temp);
			for(i=0;ch &&i < BUF_LEN;i++,temp++)
				get_user(ch,temp);
			device_write(file,(char *)ioctl_param,i,0);
			break;

		case IOCTL_GET_MSG:
			i = device_read(file,(char *)ioctl_param,99,0);
			put_user('\0',(char *)ioctl_param+i);
			break;

		case IOCTL_GET_NTH_BYTE:
			return Message[ioctl_param];
	}
	return SUCCESS;
}

struct file_operations Fops = {
	.read = device_read,
	.write = device_write,
	.unlocked_ioctl = device_ioctl,
	.open = device_open,
	.release = device_release,
};

int init_module(void)
{
	MAJOR_NUM = register_chrdev(0,DEVICE_NAME,&Fops);
	if(MAJOR_NUM < 0){
		printk("%s failed with %d\n","Sorry,registering the character device",MAJOR_NUM);
		return MAJOR_NUM;
	}
	printk("%s The major device number is %d\n","Reigster is a success",MAJOR_NUM);
	printk("If you want to talk to the device driver,\n");
	printk("you'll have to create a device file\n");
	printk("we suggest you use;\n");
	printk("mknod %s c %d 0\n",DEVICE_NAME,MAJOR_NUM);
	printk("the device file name is important,bu\n");
	printk("the ioctl program assumes that's the \n");
	printk("file you'll use.\n");
	return 0;
}

void cleanup_module(void)
{
	unregister_chrdev(MAJOR_NUM,DEVICE_NAME);
}
