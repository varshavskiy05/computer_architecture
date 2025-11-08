# SPDX-License-Identifier: GPL-3.0

# Два окремі модулі
obj-m += hello1.o
obj-m += hello2.o

# Інклуди з підкаталогу inc/, щоб #include "hello1.h" працював без шляху
ccflags-y := -I$(PWD)/inc

# Якщо KDIR не заданий — візьмемо поточне ядро
KDIR ?= /lib/modules/$(shell uname -r)/build

# Підтримка крос-компіляції через змінні середовища
all modules:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

.PHONY: all modules clean
