// SPDX-License-Identifier: GPL-2-Clause
#include <linux/init.h>
#include <linux/module.h>
#include <linux/ktime.h>
#include <linux/list.h>
#include <linux/slab.h>
#include "hello1.h"

MODULE_AUTHOR("Hrabyk Vladyslav");
MODULE_DESCRIPTION("hello1 module");
MODULE_LICENSE("Dual BSD/GPL");

struct print_time {
	struct list_head list;
	ktime_t start_time;
	ktime_t end_time;
};

static LIST_HEAD(time_list_hello);

static int __init custom_hello_init(void)
{
	pr_info("hello1 was loaded");
	return 0;
}

void custom_print_hello(unsigned int repeat_how_many)
{
	uint i;
	ktime_t start, end;
        struct print_time *time_entry;

	for (i = 0; i < repeat_how_many; i++) {
		time_entry = kmalloc(sizeof(*time_entry), GFP_KERNEL);
                start = ktime_get();
                pr_info("Hello, world!\n");
                end = ktime_get();

                time_entry->start_time = start;
                time_entry->end_time = end;
		list_add_tail(&time_entry->list, &time_list_hello);
	}
}
EXPORT_SYMBOL(custom_print_hello);

static void __exit custom_hello_exit(void)
{
	uint i = 0;
	struct print_time *entry, *temp;

	list_for_each_entry_safe(entry, temp, &time_list_hello, list) {
		pr_info("[%d] print start time = %lld; end time = %lld\n",
			i++, entry->start_time, entry->end_time);
		list_del(&entry->list);
		kfree(entry);
	}
	pr_info("hello1 module removed.\n");
}

module_init(custom_hello_init);
module_exit(custom_hello_exit);
