
ifeq ($(PARAM_FILE), )
        PARAM_FILE:=../../Makefile.param
        include $(PARAM_FILE)
endif
			
obj-m := tlv_320aic31.o
tlv_320aic31-y += tlv320aic31.o

EXTRA_CFLAGS += -I$(PWD)/ -DHI_GPIO_I2C -D$(HI_FPGA) 
EXTRA_CFLAGS += -I$(PWD)/../gpio_i2c_8b 

default:
	@make -C $(LINUX_ROOT) M=$(PWD) modules 
clean: 
	@make -C $(LINUX_ROOT) M=$(PWD) clean 
