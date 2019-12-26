#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("Dual BSD/GPL"); 

/* 	// Passing Array as parameter
	
	int paramArray[3];
	module_param_array(paramArray, int,NULL,
	S_IWUSR|S_IRUSR);
*/




int paramTest = 5;
module_param(paramTest,int,S_IRUSR|S_IWUSR);

/*	There are 5 types of permissions: 
		S_IWUSR,
		S_IRUSR, 
		S_IXUSR, 
		S_IRGRP, 
		S_WGRP

	Explanation of each permission will be decoded using below identifiers.
	In this S_I is common header.
		R = Read ,W =Write ,X= Execute
		USR =User ,GRP =Group
		Using OR ‘|’ (or operation) we can set multiple perissions at a time
*/

static int para_init(void) {
        printk(KERN_ALERT "Parameter Demo \n"); 
	printk("Value of parameter is %d",paramTest);
        return 0;
}

static void para_exit(void) {
        printk(KERN_ALERT "Exiting Parameter Demo \n"); 
}

module_init(para_init);
module_exit(para_exit);                                                                                                                       
