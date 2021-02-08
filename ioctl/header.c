#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/uaccess.h> // for communicating user space to kernel
#include <linux/kern_levels.h>
#include <linux/ioctl.h>

#define WR_DATA _IOW('a','a',int32_t*)
#define RD_DATA _IOR('a','b',int32_t*)

int main() {
	int fd;
	int32_t val;
	printf("\n IOCTL based charcter device driver..\n");
	fd = open("/dev/chr_device",O_RDWR);

	if(fd < 0) {
		printf("\nCan't open device file\n");
		return 0;
	}
	printf("Enter the data to send ..\n");
	scanf("%d",&num);
	printf("Writing value to the driver..\n");
	ioctl(fd_WR_DATA,(int32_t*)&num);
	
	printf("Reading value from driver ..\n");
	ioctl(fd, RD_DATA,(int32_t*)&val);
	
	printf("CLosing driver..\n");
	close(fd);
}
