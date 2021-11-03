#include <linux/init.h> 
#include <linux/module.h>

#include <linux/etherdevice.h>
#include <linux/ethtool.h>

#include <linux/netdevice.h>

#include <linux/delay.h>

MODULE_LICENSE( "GPL" );
MODULE_AUTHOR( "Yuri Goncharuk <lnkgyv@gmail.com>" );

#define TO_STR(ARG)	#ARG

static int dev_num = 0;
module_param(dev_num, int, S_IRUGO);

#define MQ_MAX		8
static int mq_num = 0;
module_param(mq_num, int, S_IRUGO);

static struct net_device *device_array[] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};

static int netdev_skeleton_open(struct net_device *dev)
{
	int mq_iter = 0;

	if (mq_num > 0) {
		for (mq_iter = 0; mq_iter < mq_num; mq_iter++) {
			printk(KERN_INFO "%s(%s): starting subqueue %d\n",
				__func__,
				dev->name,
				mq_iter);
			netif_start_subqueue(dev, mq_iter);
		}
	} else {
		printk(KERN_INFO "%s(%s): starting queue\n",
				__func__,
				dev->name);
		netif_start_queue(dev);
	}

	return 0;
}

static int netdev_skeleton_stop(struct net_device *dev)
{
	printk(KERN_INFO "%s(%s): stoping queue(s)\n",
				__func__,
				dev->name);
	netif_stop_queue(dev);

	return 0;
}

static int netdev_skeleton_start_xmit(struct sk_buff *skb, struct net_device *dev)
{
	if (mq_num > 0) {
		printk(KERN_INFO "%s(%s): skb for queue: %u\n",
				__func__,
				dev->name,
				skb->queue_mapping);
	} else {
		printk(KERN_INFO "%s(%s): send skb\n",
				__func__,
				dev->name);
	}

	dev_kfree_skb(skb);

	return 0;
}

static int netdev_skeleton_change_addr(struct net_device *dev, void *addr)
{
	printk(KERN_INFO "%s(%s): address changing\n",
				__func__,
				dev->name);

	if (eth_prepare_mac_addr_change(dev, addr) == -EADDRNOTAVAIL)
		return -EADDRNOTAVAIL;

	eth_commit_mac_addr_change(dev, addr);

	return 0;
}

static void netdev_skeleton_get_channels(struct net_device *dev,
			       struct ethtool_channels *ch)
{
	printk(KERN_INFO "%s(%s): get number of queues\n",
				__func__,
				dev->name);

	ch->max_combined = mq_num;
	ch->combined_count = mq_num;

}

static struct net_device_ops ndo = {
	.ndo_open = netdev_skeleton_open,
	.ndo_stop = netdev_skeleton_stop,
	.ndo_start_xmit = netdev_skeleton_start_xmit,
	.ndo_set_mac_address = netdev_skeleton_change_addr,
};

static struct ethtool_ops ethtool_ops = {
	.get_channels = netdev_skeleton_get_channels,
};

static void netdev_setup(struct net_device *dev)
{
	int octet_iter = 0;

	printk(KERN_INFO "%s(%s): setup device\n",
				__func__,
				dev->name);

	for (octet_iter = 0; octet_iter < ETH_ALEN - 1; octet_iter++) {
		dev->dev_addr[octet_iter] = (char)octet_iter;
	}

	dev->dev_addr[ETH_ALEN - 1] = (char)dev->dev_id;

	ether_setup(dev);
	dev->netdev_ops = &ndo;

	if (mq_num > 0) {
		dev->ethtool_ops = &ethtool_ops;
	}
}

static int __init netdev_skeleton_init(void)
{
	int 	dev_iter = 0;

	if (dev_num <= 0 || dev_num > (sizeof(device_array) / sizeof(device_array[0]))) {
		printk(KERN_ERR "%s: dev_num less then 0 and greater then %lu\n\
1..%lu values are allowed\n",
				__func__,
				sizeof(device_array) / sizeof(device_array[0]),
				sizeof(device_array) / sizeof(device_array[0]));

		return -1;
	}

	if (mq_num < 0 || mq_num > MQ_MAX) {
		printk(KERN_ERR "%s: mq_num_num less then 0 and greater then " TO_STR(MQ_MAX) "\n\
0.." TO_STR(MQ_MAX) " values are allowed\n", __func__);

		return -1;
	}

	for (dev_iter = 0; dev_iter < dev_num; dev_iter++) {
		if (mq_num > 0) {
			device_array[dev_iter] = alloc_netdev_mq(
				0 /* priv size */,
				"net_skel%d",
				3 /* ifname assigned by userspace */,
				netdev_setup,
				mq_num);
		} else {
			device_array[dev_iter] = alloc_netdev(
				0 /* priv size */,
				"net_skel%d",
				3 /* ifname assigned by userspace */,
				netdev_setup);
		}

		if (register_netdev(device_array[dev_iter])) {
			printk(KERN_INFO "%s: failed to register netdev_skeleton interface %d\n",
				__func__,
				dev_iter);
			free_netdev(device_array[dev_iter]);
		}
	}

	printk(KERN_INFO "%s: Module " KBUILD_MODNAME " has loaded!\n",
				__func__);

	return 0; 
}
 
static void __exit netdev_skeleton_exit(void)
{
	int dev_iter = 0;

	for (dev_iter = 0; dev_iter < dev_num; dev_iter++) {
		unregister_netdev(device_array[dev_iter]);
	}

	printk(KERN_INFO "%s: Module " KBUILD_MODNAME " has unloaded!\n",
				__func__);
}

module_init( netdev_skeleton_init );
module_exit( netdev_skeleton_exit );
