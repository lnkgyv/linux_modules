#define pr_fmt(fmt) "%s: %s: " fmt, KBUILD_MODNAME, __func__

#include <linux/init.h> 
#include <linux/module.h>

#include <linux/types.h>	/* u32 */
#include <linux/slab.h>		/* kalloc... */
#include <linux/cpumask.h>	/* cpumask_t */

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Yuri Goncharuk <lnkgyv@gmail.com>");

static int __init cpulist_init(void)
{
	cpumask_t cpus_allowed;
	u8 *buf = NULL;
	u32 len = 1024;

	pr_info("Module has loaded!\n");

	buf = kzalloc(sizeof(u8) * len, GFP_KERNEL);
	if (!buf) {
		pr_err("Can't allocate buffer\n");

		return -EPERM;
	}

	cpumask_setall(&cpus_allowed);
	cpulist_scnprintf(buf, len, &cpus_allowed);
	pr_info("Allowed cpus: %s\n", buf);

	kfree(buf);

	return 0; 
}
 
static void __exit cpulist_exit(void)
{
	pr_info("Module has unloaded!\n");
}

module_init(cpulist_init);
module_exit(cpulist_exit);
