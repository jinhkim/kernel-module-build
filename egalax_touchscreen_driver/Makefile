KERNEL_TREE_PATH?=/lib/modules/$(shell uname -r)/build
EXTRA_CFLAGS="-DDEBUG"

obj-m+=$(DRIVER_NAME).o

all: $(DRIVER_NAME).ko

hello.ko: $(DRIVER_NAME).c
	make -d -C $(KERNEL_TREE_PATH) M=$(PWD) modules

clean:
	make -d -C $(KERNEL_TREE_PATH) M=$(PWD) clean

.PHONY: all clean
