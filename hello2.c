// SPDX-License-Identifier: GPL-3.0

#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include "hello1.h"

MODULE_AUTHOR("Tymur Varshavskiy <timikinvarshav05@gmail.com>");
MODULE_DESCRIPTION("hello2: user of hello1::print_hello()");
MODULE_LICENSE("GPL");

/* Параметр модуля hello2 */
static unsigned int count = 1;
module_param(count, uint, 0444);
MODULE_PARM_DESC(count, "How many times to print 'Hello, world!' (uint)");

static int __init hello2_init(void)
{
	pr_info("hello2: loaded (count=%u)\n", count);
	print_hello_count = count;
	print_hello();	/* символи експортує hello1 */
	return 0;
}

static void __exit hello2_exit(void)
{
	pr_info("hello2: unloaded\n");
}

module_init(hello2_init);
module_exit(hello2_exit);
