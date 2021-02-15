#include <linux/init.h> 
#include <linux/module.h>

#include <linux/etherdevice.h>

#include <linux/netdevice.h>

MODULE_LICENSE( "GPL" );
MODULE_AUTHOR( "Yuri Goncharuk <lnkgyv@gmail.com>" );

static int dev_num = 0;
module_param(dev_num, int, S_IRUGO);

static struct net_device *device_array[] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
static int current_device = 0;

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

static int netdev_skeleton_change_addr(struct net_device *dev, void *addr)
{
	if (eth_prepare_mac_addr_change(dev, addr) == -EADDRNOTAVAIL)
		return -EADDRNOTAVAIL;

	eth_commit_mac_addr_change(dev, addr);

	return 0;
}

static struct net_device_ops ndo = {
	.ndo_open = netdev_skeleton_open,
	.ndo_stop = netdev_skeleton_stop,
	.ndo_start_xmit = netdev_skeleton_start_xmit,
	.ndo_set_mac_address = netdev_skeleton_change_addr,
};

static void netdev_setup(struct net_device *dev)
{
	int octet_iter;

	for (octet_iter = 0; octet_iter < ETH_ALEN - 1; octet_iter++) {
		dev->dev_addr[octet_iter] = (char)octet_iter;
	}

	dev->dev_addr[ETH_ALEN - 1] = (char)current_device;

	ether_setup(dev);
	dev->netdev_ops = &ndo;
}

static int __init netdev_skeleton_init(void)
{
	if (dev_num <= 0 || dev_num > (sizeof(device_array) / sizeof(device_array[0]))) {
		printk(KERN_ERR "dev_num less then 0 and greater then %lu\n\
1..%lu values are allowed\n", sizeof(device_array) / sizeof(device_array[0]),
			      sizeof(device_array) / sizeof(device_array[0]));

		return -1;
	}

	for (current_device = 0; current_device < dev_num; current_device++) {
		device_array[current_device] = alloc_netdev(0, "net_skel%d", 3 /* ifname assigned by userspace */, netdev_setup);

		if (register_netdev(device_array[current_device])) {
			printk(KERN_INFO "Failed to register netdev_skeleton interface %d\n", current_device);
			free_netdev(device_array[current_device]);
		}
	}

	printk(KERN_INFO "Module 'netdev_skeleton' has loaded!\n");

	return 0; 
}
 
static void __exit netdev_skeleton_exit(void)
{
	for (current_device = 0; current_device < dev_num; current_device++) {
		unregister_netdev(device_array[current_device]);
	}

	printk(KERN_INFO "Module 'netdev_skeleton' has unloaded!\n");
}

module_init( netdev_skeleton_init );
module_exit( netdev_skeleton_exit );
