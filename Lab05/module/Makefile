#path
obj-m:=path.o
current:=$(shell pwd)
kernel:=$(shell uname -r)
path:=/usr/src/linux-headers-$(kernel)
all:
    make -C $(path) M=$(current) modules
clean:
    make -C $(path) M=$(current) clean