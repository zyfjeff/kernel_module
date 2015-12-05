#ifndef CHARDEV_H
#define CHARDEV_H
#include <asm-generic/ioctl.h>
#define IO_MAGIC 'z'
#define IOCTL_SET_MSG _IOR(IO_MAGIC,0,char *)
#define IOCTL_GET_MSG _IOR(IO_MAGIC,1,char *)
#define IOCTL_GET_NTH_BYTE _IOWR(IO_MAGIC,2,int)
#define DEVICE_FILE_NAME "char_dev"
#endif
