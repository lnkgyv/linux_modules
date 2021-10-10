#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/version.h>
#include <linux/interrupt.h> /* struct tasklet_struct */

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Yuri Goncharuk <goncharuk@nicevt.ru>");
MODULE_DESCRIPTION("Angara netdev device driver");

static struct tasklet_struct tasklet;

static void tasklet_handler(unsigned long parm)
{
		printk(KERN_INFO "Hello from tasklet\n");
}

static int __init my_tasklet_init(void)
{
	unsigned long parm = 0;

	tasklet_init(&tasklet, tasklet_handler, parm);

	tasklet_schedule(&tasklet);

	return 0;
}

static void __exit my_tasklet_exit(void)
{
		tasklet_kill(&tasklet);
}

module_init(my_tasklet_init);
module_exit(my_tasklet_exit);
