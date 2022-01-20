#define pr_fmt(fmt) "%s: %s: " fmt, KBUILD_MODNAME, __func__

#include <linux/init.h> 
#include <linux/module.h>
#include <linux/workqueue.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Yuri Goncharuk <lnkgyv@gmail.com>");

unsigned counter = 0;
struct delayed_work delayed_work;

void delayed_work_handler(struct work_struct *work) {
	pr_info("Delayed work handler invoked %u times\n", ++counter);

	/* delayed worker invoke itself in every 5 sec */
	schedule_delayed_work(&delayed_work, 5 * HZ);
}

static int __init worker_skeleton_init(void)
{
	pr_info("Module " KBUILD_MODNAME " has loaded!\n");

	INIT_DELAYED_WORK(&delayed_work, delayed_work_handler);

	/* schedule delayed work in 5 sec */
	schedule_delayed_work(&delayed_work, 5 * HZ);

	return 0; 
}
 
static void __exit worker_skeleton_exit(void)
{
	pr_info("Module " KBUILD_MODNAME " has unloaded!\n");
	/* it should be called during module unloading
	 * avoiding hangup host */    
	cancel_delayed_work(&delayed_work);
}

module_init(worker_skeleton_init);
module_exit(worker_skeleton_exit);
