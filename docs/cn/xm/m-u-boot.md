## u-boot修改
* 环境：Ubuntu 18.04.2 LTS (GNU/Linux 4.15.0-54-generic x86_64)
* 参考版本：Hi3520D_SDK_V1.0.5.1


### 完整flash布局

```
|      1M       |      4M       |      11M      |
|---------------|---------------|---------------|
|     u-boot    |     kernel    |     rootfs    |
```

### u-boot布局

```
|      512K     |     256K      |    256K       |
|---------------|---------------|---------------|
|     u-boot    |    boot ENV   |    unuse      |
```


### 支持USB升级

* file: u-boot/include/configs/hi3516a.h
* 参考修改 : https://github.com/lishaoliang/opendvr/tree/master/docs/cn/xm/modify/u-boot-2010.06/include/configs/hi3520d.h

```
/* 支持自动升级 */
#define CONFIG_AUTO_UPDATE 1
```

* file: u-boot/product/hiupdate/auto_update.c
* 参考修改 : https://github.com/lishaoliang/opendvr/blob/master/docs/cn/xm/modify/u-boot-2010.06/product/hiupdate/auto_update.c

```
/* USB升级查找文件名 */
/* possible names of files on the medium. */
#define AU_FIRMWARE "u-boot.hi3520dv200"
#define AU_KERNEL   "kernel.hi3520dv200"
#define AU_ROOTFS   "rootfs.hi3520dv200"

/* USB升级flash地址设置 */
/* layout of the FLASH. ST = start address, ND = end address. */
#define AU_FL_FIRMWARE_ST 0x0
#define AU_FL_FIRMWARE_ND 0x7FFFF
#define AU_FL_KERNEL_ST   0x100000
#define AU_FL_KERNEL_ND   0x4FFFFF
#define AU_FL_ROOTFS_ST   0x500000
#define AU_FL_ROOTFS_ND   0xFFFFFF
```

### u-boot代码导读

* C代码入口函数

```
file: u-boot/arch/arm/lib/board.c
func: void start_armboot (void)
```

* u-boot环境变量存储于flash位置及大小

```
file: u-boot/include/configs/hi3516a.h
#define CONFIG_ENV_OFFSET  0x80000 /* environment starts here */
#define CONFIG_ENV_SIZE    0x40000 /* include ENV_HEADER_SIZE */
```


* USB升级调用流程

```
start_armboot
misc_init_r
do_auto_update
```

* u-boot打印读取文件名

```
file: u-boot/fs/fat/fat.c
func: long file_fat_read(const char *filename, void *buffer, unsigned long maxsize)

printf("reading %s\n",filename);
```
