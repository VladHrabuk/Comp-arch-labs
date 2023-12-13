// SPDX-License-Identifier: GPL-2-Clause
#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include "hello1.h"

MODULE_AUTHOR("Hrabyk Vladyslav");
MODULE_DESCRIPTION("Hello world message");
MODULE_LICENSE("Dual BSD/GPL");

static uint repeat_how_many = 1;
module_param(repeat_how_many, uint, 0444);
MODULE_PARM_DESC(repeat_how_many, "how many times the greeting will be printed");


static int __init custom_hello_init(void)
{
	if (repeat_how_many == 0 || (repeat_how_many >= 5 && repeat_how_many <= 10)) {
		pr_warn("Warning: repeat_how_many is %u\n", repeat_how_many);
	} else if (repeat_how_many > 10) {
		pr_err("Error: repeat_how_many is more then 10\n");
		return -EINVAL;
	}
	custom_print_hello(repeat_how_many);
	return 0;
}

static void __exit custom_hello_exit(void)
{
	pr_info("Module hello2 removed: Bye world.\n");
}

module_init(custom_hello_init);
module_exit(custom_hello_exit);
