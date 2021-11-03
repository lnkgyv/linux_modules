#include <linux/module.h>

MODULE_LICENSE( "GPL" );
MODULE_AUTHOR( "Yuri Goncharuk <lnkgyv@gmail.com>" );

extern int exporter_func1(void) {
	printk(KERN_INFO "Hello from %s\n", __func__);

	return 0;
};
EXPORT_SYMBOL(exporter_func1);
