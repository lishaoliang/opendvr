
ifeq ($(PARAM_FILE), )
		PARAM_FILE:=../../Makefile.param
		include $(PARAM_FILE)
endif

obj-m := ssp.o
ssp-y += hi_ssp.o

#EXTRA_CFLAGS += -D$(HI_FPGA) -D$(CHIP_ARCH)
#EXTRA_CFLAGS+=-I$(PWD)/../gpio-i2c-8bit
#EXTRA_CFLAGS += -DSSP_USE_GPIO_DO_CS
 
default:	
	$(CC) -g -Wall -o ssp_read ssp_read.c strfunc.c	
	$(CC) -g -Wall -o ssp_write ssp_write.c	strfunc.c
	@make -C $(LINUX_ROOT) M=$(PWD) modules
	rm *.o modules.* *.symvers *.mod.c
clean:
	@make -C $(LINUX_ROOT) M=$(PWD) clean
	@rm -rf ssp_read ssp_write
