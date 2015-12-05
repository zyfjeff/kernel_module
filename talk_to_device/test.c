#include "chardev1.h"
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
void ioctl_set_msg(int file_desc,char *message)
{
	int ret_val;
	ret_val = ioctl(file_desc,IOCTL_SET_MSG,message);
	if (ret_val < 0) {
		printf("ioctl_set_msg failed:%d\n",ret_val);
		exit(-1);
	}
}
void ioctl_get_msg(int file_desc)
{
	int ret_val;
	char message[100];
	ret_val = ioctl(file_desc,IOCTL_GET_MSG,message);
	if (ret_val < 0){
		printf("ioctl_get_msg failed:%d\n",ret_val);
		exit(-1);
	}
	printf("get_msg message:%s\n",message);

}

void ioctl_get_nth_byte(int file_desc)
{
	int i;
	char c='m'; //最好给个处置，在我的机器上居然好几次初值都是0.导致while循环跳出
	printf("get_nth_byte message:");
	i = 0;
	while(c != 0){
		c = ioctl(file_desc,IOCTL_GET_NTH_BYTE,i++);
		if(c < 0){
			printf("ioctl_get_nth_byte failed at the %d'th byte:\n",i);
			exit(-1);
		}
		putchar(c);
	}
	putchar('\n');

}

int main()
{
	int file_desc,ret_val;
	char *msg = "Message passwd by ioctl\n";
	file_desc = open("/dev/char_dev",0);
	if(file_desc < 0) {
		printf("can't open device file: %s\n",DEVICE_FILE_NAME);
		exit(-1);
	}
   ioctl_get_nth_byte(file_desc);
   //ioctl_get_msg(file_desc);
   //ioctl_set_msg(file_desc,msg);

   close(file_desc);
}
