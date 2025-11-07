// SPDX-License-Identifier: GPL-3.0
/*
 * Copyright (c) 2017, GlobalLogic Ukraine LLC
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *    This product includes software developed by the GlobalLogic.
 * 4. Neither the name of the GlobalLogic nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY GLOBALLOGIC UKRAINE LLC ``AS IS`` AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL GLOBALLOGIC UKRAINE LLC BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/moduleparam.h>
#include <linux/slab.h>
#include <linux/list.h>
#include <linux/ktime.h>

MODULE_AUTHOR("Tymur Varshavskiy <timikinvarshav05@gmail.com>");
MODULE_DESCRIPTION("Hello, world with param + timestamps list");
MODULE_LICENSE("GPL");

static unsigned int hello_count = 1;
module_param(hello_count, uint, 0444);
MODULE_PARM_DESC(hello_count, "How many times to print 'Hello, world!' (uint)");

struct hello_item {
	struct list_head list;
	ktime_t ts;
};

static LIST_HEAD(hello_head);

static int __init hello_init(void)
{
	unsigned int i;

	if (hello_count > 10) {
		pr_err("hello: invalid hello_count=%u (>10). Aborting.\n",
			hello_count);
		return -EINVAL;
	}

	if (hello_count == 0 || (hello_count >= 5 && hello_count <= 10))
		pr_warn("hello: warning: hello_count=%u (allowed with warning)\n",
			hello_count);

	for (i = 0; i < hello_count; i++) {
		struct hello_item *item = kmalloc(sizeof(*item), GFP_KERNEL);

		if (!item) {
			pr_err("hello: kmalloc failed at i=%u\n", i);
			return -ENOMEM;
		}

		item->ts = ktime_get();
		list_add_tail(&item->list, &hello_head);
		pr_emerg("Hello, world! (%u/%u)\n", i + 1, hello_count);
	}

	return 0;
}

static void __exit hello_exit(void)
{
	struct hello_item *pos, *n;

	list_for_each_entry_safe(pos, n, &hello_head, list) {
		s64 ns = ktime_to_ns(pos->ts);

		pr_info("hello: event time (ns): %lld\n", (long long)ns);
		list_del(&pos->list);
		kfree(pos);
	}
}

module_init(hello_init);
module_exit(hello_exit);
