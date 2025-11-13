// SPDX-License-Identifier: GPL-3.0


#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/slab.h>
#include <linux/list.h>
#include <linux/ktime.h>
#include "hello1.h"

MODULE_AUTHOR("Tymur Varshavskiy <timikinvarshav05@gmail.com>");
MODULE_DESCRIPTION("hello1: print_hello() with timing");
MODULE_LICENSE("GPL");

/* Елемент списку з двома мітками часу */
struct hello_item {
	struct list_head list;
	ktime_t t_before;
	ktime_t t_after;
};

static LIST_HEAD(hello_head);

/*
 * Цей символ змінює hello2 перед викликом
 * print_hello()
 */
unsigned int print_hello_count;
EXPORT_SYMBOL(print_hello_count);

void print_hello(void)
{
	unsigned int i;

	if (print_hello_count > 10) {
		pr_err("hello1: invalid count=%u (>10)\n", print_hello_count);
		return;
	}

	if (print_hello_count == 0 ||
	    (print_hello_count >= 5 && print_hello_count <= 10))
		pr_warn("hello1: warning count=%u is allowed with warning\n",
			print_hello_count);

	for (i = 0; i < print_hello_count; i++) {
		struct hello_item *item = kmalloc(sizeof(*item), GFP_KERNEL);

		if (!item) {
			pr_err("hello1: kmalloc failed at i=%u\n", i);
			return;
		}

		item->t_before = ktime_get();
		pr_info("Hello, world! (%u/%u)\n", i + 1, print_hello_count);
		item->t_after = ktime_get();

		list_add_tail(&item->list, &hello_head);
	}
}
EXPORT_SYMBOL(print_hello);

static int __init hello1_init(void)
{
	pr_info("hello1: loaded\n");
	return 0;
}

static void __exit hello1_exit(void)
{
	struct hello_item *pos, *n;
	unsigned int idx = 0;

	pr_debug("hello1: about to dump timing list (count unknown at compile-time)\n");
	list_for_each_entry_safe(pos, n, &hello_head, list) {
		s64 delta_ns = ktime_to_ns(ktime_sub(pos->t_after,
						     pos->t_before));
		pr_debug("hello1: print #%u took %lld ns\n", ++idx,
			(long long)delta_ns);
		list_del(&pos->list);
		kfree(pos);
	}
	pr_debug("hello1: finished dumping timing list, freeing nodes done\n");


	pr_info("hello1: unloaded\n");
}

module_init(hello1_init);
module_exit(hello1_exit);

