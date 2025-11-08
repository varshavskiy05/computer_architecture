/* SPDX-License-Identifier: GPL-3.0 */
#ifndef HELLO1_H
#define HELLO1_H

#include <linux/ktime.h>

/* Публічна функція, яку викликає hello2 */
void print_hello(void);

extern unsigned int print_hello_count;

#endif /* HELLO1_H */
