#include <linux/init.h> 
#include <linux/module.h>

#include <linux/netdevice.h>

MODULE_LICENSE( "GPL" );
MODULE_AUTHOR( "Yuri Goncharuk <lnkgyv@gmail.com>" );

static struct net_device *dev;

static int netdev_skeleton_open(struct net_device *dev)
{
	printk(KERN_INFO "netdev_skeleton_open(%s)\n", dev->name);
	netif_start_queue(dev);

	return 0;
}

static int netdev_skeleton_stop(struct net_device *dev)
{
	printk(KERN_INFO "netdev_skeleton_stop(%s)\n", dev->name);
	netif_stop_queue(dev);

	return 0;
}

static int netdev_skeleton_start_xmit(struct sk_buff *skb, struct net_device *dev)
{
	printk(KERN_INFO "netdev_skeleton_start_xmit(%s)\n", dev->name);
	dev_kfree_skb(skb);

	return 0;
}

static struct net_device_ops ndo = {
	.ndo_open = netdev_skeleton_open,
	.ndo_stop = netdev_skeleton_stop,
	.ndo_start_xmit = netdev_skeleton_start_xmit,
};

static void netdev_setup(struct net_device *dev)
{
	int octet_iter;

	for (octet_iter = 0; octet_iter < ETH_ALEN; octet_iter++) {
		dev->dev_addr[octet_iter] = (char)octet_iter;
	}

	ether_setup(dev);
	dev->netdev_ops = &ndo;
}

static int __init netdev_skeleton_init(void)
{
	dev = alloc_netdev(0, "net_skel%d", 3 /* ifname assigned by userspace */, netdev_setup);
	if (register_netdev(dev)) {
		printk(KERN_INFO "Failed to register netdev_skeleton driver\n");
		free_netdev(dev);

		return -1;
	}

	printk(KERN_INFO "Module 'netdev_skeleton' has loaded!\n");

	return 0; 
}
 
static void __exit netdev_skeleton_exit(void)
{
	unregister_netdev(dev);
	printk(KERN_INFO "Module 'netdev_skeleton' has unloaded!\n");
}

module_init( netdev_skeleton_init );
module_exit( netdev_skeleton_exit );
