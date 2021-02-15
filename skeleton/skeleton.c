#include <linux/init.h> 
#include <linux/module.h>

MODULE_LICENSE( "GPL" );
MODULE_AUTHOR( "Yuri Goncharuk <lnkgyv@gmail.com>" );

static int __init skeleton_init( void )
{
	printk(KERN_INFO "Module 'skeleton' has loaded!\n");

	return 0; 
}
 
static void __exit skeleton_exit( void )
{
	printk(KERN_INFO "Module 'skeleton' has unloaded!\n");
}

module_init( skeleton_init );
module_exit( skeleton_exit );
