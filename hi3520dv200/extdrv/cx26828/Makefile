ifeq ($(PARAM_FILE), )
		PARAM_FILE:=../Makefile.param
		include $(PARAM_FILE)
endif


obj-m := cx26828.o 
cx_26828-y += cx26828.o


EXTRA_CFLAGS += -Wall -O2 -I$(PWD)/ -DHI_GPIO_I2C

default:
	@make -C $(LINUX_ROOT) M=$(PWD) modules
clean: 
	@make -C $(LINUX_ROOT) M=$(PWD) clean







