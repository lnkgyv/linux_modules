#define pr_fmt(fmt) "%s: %s: " fmt, KBUILD_MODNAME, __func__

#include <linux/init.h> 
#include <linux/module.h>
#include <linux/workqueue.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Yuri Goncharuk <lnkgyv@gmail.com>");

unsigned counter = 0;
struct work_struct work;

void work_handler(struct work_struct *work) {
	pr_info("Work handler invoked %u times\n", ++counter);
}

static int __init worker_skeleton_init(void)
{
	pr_info("Module " KBUILD_MODNAME " has loaded!\n");

	INIT_WORK(&work, work_handler);

	schedule_work(&work);
	flush_work(&work);

	schedule_work(&work);
	flush_work(&work);

	return 0; 
}
 
static void __exit worker_skeleton_exit(void)
{
	pr_info("Module " KBUILD_MODNAME " has unloaded!\n");
}

module_init(worker_skeleton_init);
module_exit(worker_skeleton_exit);
