#include <linux/init.h> 
#include <linux/module.h>

#include <linux/list.h>		/* struct list_head */
#include <linux/types.h>	/* u32 */
#include <linux/slab.h>		/* kalloc... */

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Yuri Goncharuk <lnkgyv@gmail.com>");

#define CELLS	20

struct cell {
	u32 val;
	struct list_head cell_list;
};

struct list_head cells;

static void free_list(void) {
	struct cell *curr,
		    *curr_n;

	list_for_each_entry_safe (curr, curr_n, &cells, cell_list) {
		list_del(&curr->cell_list);
		kfree(curr);
	}
}

static void print_list(void) {
	struct cell *curr;

	printk(KERN_INFO "Elements of list\n");

	list_for_each_entry (curr, &cells, cell_list) {
		printk(KERN_INFO "%u\n", curr->val);
	}
}

static int init_list(void) {
	struct cell *el = NULL;
	u32 cells_iter = 0;

	/* Init head of list */
	INIT_LIST_HEAD(&cells);

	for (cells_iter = 0; cells_iter < CELLS; cells_iter++) {
		el = kzalloc(sizeof(struct cell), GFP_KERNEL);
		if (!el) {
			printk(KERN_ERR "There is no enough memory for cell node\n");

			goto init_list_no_mem;
		}
		el->val = cells_iter;

		/* Init list node */
		INIT_LIST_HEAD(&el->cell_list);
		list_add(&el->cell_list, &cells);
	}

	return 0;

init_list_no_mem:
	free_list();

	return -ENOMEM;
}

static int __init skeleton_init(void)
{
	printk(KERN_INFO "Module 'list' has loaded!\n");

	init_list();

	print_list();

	return 0; 
}
 
static void __exit skeleton_exit(void)
{
	free_list();
	
	printk(KERN_INFO "Module 'list' has unloaded!\n");
}

module_init(skeleton_init);
module_exit(skeleton_exit);
