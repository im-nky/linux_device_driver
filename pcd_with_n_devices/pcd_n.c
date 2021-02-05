/* 'pcd' means psuedo character device */
/* Description pcd driver for multiple devices */

/* Header Section */
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

#define NUM_OF_DEVICES 4

#define mem_size_pcdev1 1024
#define mem_size_pcdev2 512
#define mem_size_pcdev3 1024
#define mem_size_pcdev4 512

/* permission modes */
#define RDONLY 0x01
#define WRONLY 0x10
#define RDWR   0x11

/* Device i.e. pseudo character device here */
char dev_buffer_1[mem_size_pcdev1];		
char dev_buffer_2[mem_size_pcdev2];
char dev_buffer_3[mem_size_pcdev3];
char dev_buffer_4[mem_size_pcdev4];

/* Note: In case of structure order matters i.e. define a structure then use it*/
/* Device private data structure*/
struct pcdev_private_data {
        char * buffer;
        unsigned size;
        const char * sr_num;
        int perm;
        struct cdev cdev;
};

/* Driver private data structure */
struct pcdrv_private_data {
	/* class decleration to add class in '/sysfs' */
	struct class *pcd_class;
	
	/* Structure for character devices to deal with VFS*/
	struct cdev pcd_cdev;

	/* structure to hold device number (MAJOR+MINOR) */
	dev_t dev;
	int total_devices;
	struct pcdev_private_data pcdev_data[NUM_OF_DEVICES];
};

/* Variable of pcdrv_private_data */
struct pcdrv_private_data pcdrv_data  = {
        .total_devices = NUM_OF_DEVICES,
	.pcdev_data = {
		[0] = {
			.buffer = dev_buffer_1,
			.size = mem_size_pcdev1,
			.sr_num = "PCDEV__1",
			.perm = RDONLY
		},
		[1] = {
                        .buffer = dev_buffer_2,
                        .size = mem_size_pcdev2,
                        .sr_num = "PCDEV__2",
                        .perm = WRONLY
                },
		[2] = {
                        .buffer = dev_buffer_3,
                        .size = mem_size_pcdev3,
                        .sr_num = "PCDEV__3",
                        .perm = RDWR
                },
		[3] = {
                        .buffer = dev_buffer_4,
                        .size = mem_size_pcdev4,
                        .sr_num = "PCDEV__4",
                        .perm = RDWR
                }	
	}
};

/*Entry and Exit Points Prototype*/
static int __init chr_driver_init(void);
static void __exit chr_driver_exit(void);

/* File Operation functions*/
static int pcd_open(struct inode *inode, struct file *file);
static int pcd_release(struct inode *inode, struct file *file);
static ssize_t pcd_read(struct file *filp, char __user *buf, size_t len, loff_t *off);
static ssize_t pcd_write(struct file *filp, const char __user *buf, size_t len, loff_t *off);
static loff_t pcd_lseek(struct file *filp, loff_t off, int whence);

/* File operation structure initialization according to C99 structure */
struct file_operations fops = {
	.owner = THIS_MODULE,
	.read = pcd_read,
	.write = pcd_write,
	.open = pcd_open,
	.release = pcd_release,
	.llseek = pcd_lseek,
};

int check_permission(int dev_parm, int acc_mode) {
	
	if(dev_parm == RDWR)
		return 0;
	/* Ensures read only access */
	if((dev_parm == RDONLY) && ((acc_mode & FMODE_READ) && !(acc_mode & FMODE_WRITE)))
		return 0;
	
	/* Ensures write only access */
	if((dev_parm == WRONLY) && ((acc_mode & FMODE_READ) && !(acc_mode & FMODE_WRITE)))
		return 0;

	return -EPERM;
}

/* file_operation definition Section */
static int pcd_open(struct inode *inode, struct file *file) {
	int ret;
	int minor_n;
	struct pcdev_private_data *pcdev_data;

	/*Creating Physical Memory Dynamically
	if((kernel_buffer = kmalloc(mem_size, GFP_KERNEL))==0) {
		printk(KERN_INFO"can't allocate the memory to kernel\n");
		return -1;
	}*/

	/* Find out which device file open was attempted by the user space */
	
	minor_n = MINOR(inode->i_rdev);
	pr_info("minor_access = %d\n",minor_n);

	/* get device's private data structure */
	pcdev_data =  container_of(inode->i_cdev,struct pcdev_private_data,cdev);

	/* to supply device data to other methods of the driver */
	file->private_data = pcdev_data;

	/* Check Permission */
	ret = check_permission(pcdev_data->perm, file->f_mode);
	
	(!ret)?pr_info("Device Open Successful\n"):pr_info("Device Open Failed\n");

	printk(KERN_INFO"Device FIle opened..\n");
	return 0;
} 

static int pcd_release(struct inode *inode, struct file *file) {
	/* kfree(kernel_buffer); */
	printk(KERN_INFO"Device File Closed\n");
	return 0;
}

static ssize_t pcd_read(struct file *filp, char __user *buf, size_t len, loff_t *offset) {
	
	struct pcdev_private_data *pcdev_data = (struct pcdev_private_data *)filp->private_data;
	
	int max_size = pcdev_data->size;

	/* Adjust the length of File */
        if((*offset + len) > max_size)
                len = max_size - *offset;

        /* copy to user */
        if(copy_to_user(buf, pcdev_data->buffer+(*offset), len)) {
                return -EFAULT;
        }

        /* Update the current file position */
        *offset += len;

        printk(KERN_INFO"Data read: DOne\n");
        return len;
}

static ssize_t pcd_write(struct file *filp, const char __user *buf, size_t len, loff_t *offset) {
	
	struct pcdev_private_data *pcdev_data = (struct pcdev_private_data *)filp->private_data;
	
	int max_size = pcdev_data->size;

	if((*offset + len) > max_size)
		len = max_size - *offset;

	if(!len) {
		pr_err("No Space Left on the device\n");
		return -ENOMEM;
	}

	if(copy_from_user(pcdev_data->buffer+(*offset), buf, len)) {
		return -EFAULT;
	}

	*offset += len;

	printk(KERN_INFO"Data written successfully\n");
	return len;
}

loff_t pcd_lseek(struct file * filp, loff_t offset, int whence) {
	
	struct pcdev_private_data *pcdev_data = (struct pcdev_private_data *)filp->private_data;

        int max_size = pcdev_data->size;

	loff_t temp;

	switch(whence) {
		case SEEK_SET:
			if((offset > max_size) || (offset < 0))
				return -EINVAL;
			filp->f_pos = offset;
			break;	
		case SEEK_CUR:
			temp = filp->f_pos + offset;
			if((temp > max_size) || (temp < 0))
                                return -EINVAL;
                        filp->f_pos = temp;
                        break;
		case SEEK_END:
			temp = max_size + offset;
                        if((temp > max_size) || (temp < 0))
                                return -EINVAL;
                        filp->f_pos = temp;
                        break;
		default:
                        return -EINVAL;
	}
	return filp->f_pos;
}

/* File Operation Ends */


/* Module Init Definition */
static int __init chr_driver_init(void) {
	int i = 0;

	/* Allocating memory region to character devices*/
	if((alloc_chrdev_region(&pcdrv_data.dev,0,NUM_OF_DEVICES,"pcd_device"))<0) {
		printk(KERN_INFO"Can't allocate major number\n");
		goto out;
	}

	/* Creating struct class in '/sys' i.e. '/sys/class/pcd_class' */
         if((pcdrv_data.pcd_class = class_create(THIS_MODULE,"pcd_class")) == NULL) {
		printk(KERN_INFO"can't create the struct class..\n");
                goto cdev_del;
        }

	for(i=0;i<NUM_OF_DEVICES;i++) {
		
		printk(KERN_INFO"MAJOR = %d :MINOR = %d\n", MAJOR(pcdrv_data.dev+i), MINOR(pcdrv_data.dev+i));

		/*creating cdev structure*/
		cdev_init(&pcdrv_data.pcdev_data[i].cdev, &fops);

		/*Adding character deivce to the system i.e. on '/dev' Directory */
		if((cdev_add(&pcdrv_data.pcdev_data[i].cdev, pcdrv_data.dev+i,1)) < 0){
			printk(KERN_INFO"Can't add device to the system\n");
			goto unreg_chrdev;
		}
	
		/* Creating Device in '/sys' i.e. '/sys/class/pcd_class/pcd_device'*/
		if((device_create(pcdrv_data.pcd_class,NULL,pcdrv_data.dev+i,NULL,"pcdev-%d",i+1)) == NULL) {
			printk(KERN_INFO"can't create the device\n");	
			goto class_del;
		}
	}

	printk(KERN_INFO"pcd_driver Inserted Sucessfully...");
	return 0;

cdev_del:
	for (;i>=0;i--)
		cdev_del(&pcdrv_data.pcdev_data[i].cdev);
class_del:
		class_destroy(pcdrv_data.pcd_class);
unreg_chrdev: 
		unregister_chrdev_region(pcdrv_data.dev,NUM_OF_DEVICES);
		return -1;
out:
		pr_info("Module Insertion Failed\n");
		return -1;
}

/* Module Exit Definition */
void __exit chr_driver_exit(void) {
	int i = 0;
	device_destroy(pcdrv_data.pcd_class,pcdrv_data.dev);
	
	for(i=0;i<NUM_OF_DEVICES;i++) {
		device_destroy(pcdrv_data.pcd_class,pcdrv_data.dev+i);
		cdev_del(&pcdrv_data.pcdev_data[i].cdev);
	}
	class_destroy(pcdrv_data.pcd_class);
	unregister_chrdev_region(pcdrv_data.dev,NUM_OF_DEVICES);
	printk(KERN_INFO "Device driver removed successfully\n");
}

/* Module Entry and Exit Point Registration */
module_init(chr_driver_init);
module_exit(chr_driver_exit);

/* Author Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Nitin Kumar Yadav <nky.mnnit@gmail.com>");
MODULE_DESCRIPTION("Psuedo Character Device Driver for Multiple Devices");
