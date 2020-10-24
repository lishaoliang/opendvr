ifeq ($(PARAM_FILE), )
		PARAM_FILE:=../Makefile.param
		include $(PARAM_FILE)
endif
obj-m := nvp6114_ex.o
nvp6114_ex-objs := nvp6114_drv.o coax_protocol.o video.o motion.o audio.o


EXTRA_CFLAGS += -I$(PWD)/../gpio_i2c
default:
	@make -C $(LINUX_ROOT) M=$(PWD) modules 

clean: 
	@make -C $(LINUX_ROOT) M=$(PWD) clean 


