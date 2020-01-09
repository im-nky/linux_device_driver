#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>

static dev_t first; //globle variable for the device no
static struct cdev c_dev; //globle var for char struct
static struct class *cl; //globle var for device class

static int my_open(struct inode *i, struct file *f) {
	printk(KERN_INFO "Driver:open()\n");
	return 0;
}

static int my_close(struct inode *i , struct file *f) {
	printk(KERN_INFO "Driver:close()\n");
	return 0;
}

static ssize_t my_read(struct file *f,char __user *buf,size_t len,loff_t *off) {
	printk(KERN_INFO "Driver:read()\n");
	return 0;
}

static int my_write(struct file *f,const char __user *buf,size_t len,loff_t *off) {
	printk(KERN_INFO "Driver:write()\n");
	return len;
}

static struct file_operations pugs_fops=
{
	.owner = THIS_MODULE,
	.open = my_open,
	.release =my_close,
	.read =my_read,
	.write =my_write
};

static int __init ofcd_init(void) /*const*/ {
	printk(KERN_INFO "NAMSAKAR:ofd reg");
	if (alloc_chrdev_region(&first, 0, 3,"Nitin") < 0) {
		return -1;
	}
	if ((cl = class_create(THIS_MODULE, "chardrv")) == NULL) {
		unregister_chrdev_region(first,1);
	return -1;
}

if (device_create(cl, NULL ,first, NULL, "mynull") == NULL) {
	class_destroy(cl);
	unregister_chrdev_region(first,1);
	return -1;
}

cdev_init(&c_dev, &pugs_fops);

if (cdev_add(&c_dev, first, 1)== -1) {
	device_destroy(cl,first);
	class_destroy(cl);
	unregister_chrdev_region(first,1);
	return -1;
	}
	return 0;
}

static void __exit ofcd_exit(void)/*Destructor*/
{
	cdev_del(&c_dev);
	device_destroy(cl, first);
	class_destroy(cl);
	unregister_chrdev_region(first,1);
	printk(KERN_INFO "by Gn:ofd unreg");
}

module_init(ofcd_init);
module_exit(ofcd_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("MJ");
MODULE_DESCRIPTION("OUR FIRST character DRIVER:automatic");
