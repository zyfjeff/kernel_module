#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>

int procfile_read(char *buffer ,char **buffer_location ,off_t offset,int buffer_length ,int *eof ,void *data)
{
	int len = 0;
	static int count = 1;
	printk(KERN_INFO "inside /proc/test1 :procfile_read.\n");

//	if(offset > 0){
//		printk(KERN_INFO "offset %d :/proc/test1:profile_read,wrote %d Bytes\n",(int)(offset),len);
//		*eof = 0;
//		return len;
//	}
	len = sprintf(buffer,"For the %d %s time,go away!\n",count,
			(count % 100 > 10 && count % 100 <14)?"th":
			(count % 10 == 1)?"st":
			(count % 10 == 2)?"nd":
			(count % 10 == 3)?"rd":"th");
	count++;
	printk(KERN_INFO "leasving /proc/test1 :profile_read,wrote %d Bytes.\n",len);

	if(len <= offset + buffer_length)
		*eof = 1;
	*buffer_location = buffer + offset;
	len -= offset;
	if(len > buffer_length)
		len = buffer_length;
	if(len<0)
			 len = 0;

	return len;
}

struct proc_dir_entry *Our_Proc_File;
int init_module(void)
{
	int rv = 0;
	Our_Proc_File = create_proc_entry("test1",0644,NULL);
	Our_Proc_File->read_proc = procfile_read;
	Our_Proc_File->mode = S_IFREG |S_IRUGO;
	Our_Proc_File->uid = 0;
	Our_Proc_File->gid = 0;
	Our_Proc_File->size = 37;
	printk(KERN_INFO "Trying to create /rpoc/test1:\n");
	if(Our_Proc_File == NULL){
		rv = -ENOMEM;
		remove_proc_entry("test1",NULL);
		printk(KERN_INFO "Error:Could not initialize /proc/test1.\n");
	}else{
		printk(KERN_INFO "Success!\n");
	}
	return rv;
}

void cleanup_module(void)
{
	remove_proc_entry("test1",NULL);
	printk(KERN_INFO "/proc/test1 has been removed.\n");
}
