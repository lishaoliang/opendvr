
./mkfs.jffs2 -d ./rootfs_uclibc -l -e 0x10000 -o rootfs.jffs2
./mkimage -A arm -T filesystem -C none -n hirootfs -d rootfs.jffs2 rootfs.bin
