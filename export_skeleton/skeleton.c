#include <linux/init.h> 
#include <linux/module.h>

#include "exporter.h"

MODULE_LICENSE( "GPL" );
MODULE_AUTHOR( "Yuri Goncharuk <lnkgyv@gmail.com>" );

static int __init skeleton_init( void )
{
	printk(KERN_INFO "Module " KBUILD_MODNAME " has loaded!\n");

	exporter_func1();

	return 0; 
}
 
static void __exit skeleton_exit( void )
{
	printk(KERN_INFO "Module " KBUILD_MODNAME " has unloaded!\n");
}

module_init( skeleton_init );
module_exit( skeleton_exit );
