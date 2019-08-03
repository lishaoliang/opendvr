#!/bin/bash

if [ -d ./rootfs_uclibc ]; then 
	echo "find rootfs_uclibc"
else 
	tar -vxzf ./rootfs_uclibc.tgz; 
fi

./mkfs.jffs2 -l -e 0x10000 -d ./rootfs_uclibc -o rootfs.jffs2
./mkimage -A arm -T filesystem -C none -n hirootfs -d rootfs.jffs2 rootfs.hi3520dv200
