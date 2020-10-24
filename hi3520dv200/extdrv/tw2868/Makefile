
ifeq ($(PARAM_FILE), )
		PARAM_FILE:=../../Makefile.param
		include $(PARAM_FILE)
endif

obj-m := tw2868.o
tw_2868-y += tw2868.o

EXTRA_CFLAGS += -D$(HI_FPGA) 
#EXTRA_CFLAGS+=-I$(PWD)/../gpio-i2c -I$(PWD)/../gpio-i2c1
EXTRA_CFLAGS += -I$(PWD)/../gpio-i2c_8b

default:
	@make -C $(LINUX_ROOT) M=$(PWD) modules
clean: 
	@make -C $(LINUX_ROOT) M=$(PWD) clean



