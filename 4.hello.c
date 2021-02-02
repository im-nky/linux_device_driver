/* Null Driver */ 
/*
	The header conatins the definition of the functions used in this module. 
	module_init() and module_exit() are defined in this file.
*/
#include <linux/init.h>

/*	-This header is included to add support for dynamic loading of module into kernel.
	-It also contains the definitions of the special macros used in this module.
	-Special macro like MODULE_LICENSE is defined in this header.
*/
#include <linux/module.h>

MODULE_LICENSE("Dual BSD/GPL");	/*This macro is used to tell the kernel that this module bears a free license; 
				without such a decleration, the kernel complains ehen the module is loaded. */

static int hello_init(void) {
	printk(KERN_ALERT "Hello, World\n");	/*This is similar to printf of C*/
	return 0;
}

static void hello_exit(void) {
	printk(KERN_ALERT "Goodbye, Cruel World\n"); /*KERN_ALERT is highest priority message on console*/
}

/*
	Module_init() is a macro which defines the function to be called at module insertion time or 
	at system boot time.

	MOdule_exit is a macro which defines the function to be called at module removal time. This 
	function also called clean up function.
*/

module_init(hello_init);
module_exit(hello_exit);	/* Kernel Modules must always conatin these two functions init and cleanup module. */

/*	Steps to Compile

	1. Use 'make'
	2. run 'insmod hello.ko' / 'modsprobe hello.ko' 
	3. run 'dmesg'	// To see kernel buffer
	4. run 'rmmod hello.ko'

*/
