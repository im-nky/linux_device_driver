#include <linux/module.h>       // Needed by all modules
#include <linux/kernel.h>       // KERN_INFO
#include <linux/sched.h>        // for_each_process, pr_info
#include <linux/sched/signal.h>

MODULE_LICENSE("GPL");
static int test_init(void) {
	struct task_struct *task;
	for_each_process(task) {
		printk("process Name :%s\t PID:%d\t Process State:%ld\n",
				task->comm,task->pid, task->state);
	}
	return 0;
}
static void test_exit(void) {
	printk(KERN_INFO "Clearing up.\n");
}

module_init(test_init);
module_exit(test_exit);
