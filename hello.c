// SPDX-License-Identifier: GPL-2-Clause
#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/moduleparam.h>
#include <linux/ktime.h>
#include <linux/list.h>
#include <linux/slab.h>

MODULE_AUTHOR("Hrabyk Vladyslav");
MODULE_DESCRIPTION("Custom Hello world module");
MODULE_LICENSE("Dual BSD/GPL");

static uint repeat_how_many = 1;
module_param(repeat_how_many, uint, 0444);
MODULE_PARM_DESC(repeat_how_many, "Number of times the message is printed");

struct message_log {
	struct list_head list;
	ktime_t log_time;
};

static LIST_HEAD(message_log_list);

static int __init custom_hello_init(void)
{
	if (repeat_how_many == 0 || (repeat_how_many >= 5 && repeat_how_many <= 10)) {
		printk(KERN_WARNING "Warning: repeat_how_many is %d\n", repeat_how_many);
	} else if (repeat_how_many > 10) {
		printk(KERN_WARNING "Error: repeat_how_many is more then 10\n");
		return -EINVAL;
	}

	uint i;

	for (i = 0; i < repeat_how_many; i++) {
		struct message_log *msg = kmalloc(sizeof(*msg), GFP_KERNEL);
		msg->log_time = ktime_get();
		list_add_tail(&msg->list, &message_log_list);
		printk(KERN_INFO "[%u] Hello world!\n", i);
	}
	return 0;
}

static void __exit custom_hello_exit(void)
{
	uint i = 0;
	struct message_log *entry, *temp;

	list_for_each_entry_safe(entry, temp, &message_log_list, list) {
		printk(KERN_INFO "[%u] Time: %lld\n", i++, ktime_to_ns(entry->log_time));
		list_del(&entry->list);
		kfree(entry);
	}
	printk(KERN_INFO "Module removed: Bye world.\n");
}

module_init(custom_hello_init);
module_exit(custom_hello_exit);


