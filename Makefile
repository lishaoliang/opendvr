# make

SHELL = /bin/bash
PWD = `pwd`

# 编译目标: "dopi3516dv3", "dopi16ev2", "ahb7004t"
target ?= dopi3516dv3

ifeq ($(target), dopi3516dv3)
	OS := linux
	ARCH := arm
	TAGS := debug normal hi3516dv300
	MY_TOOL_CHAIN := arm-himix200-linux-
else ifeq ($(target), dopi16ev2)
	OS := linux
	ARCH := arm
	TAGS := debug normal hi3516ev200
	MY_TOOL_CHAIN := arm-himix100-linux-
else ifeq ($(target), ahb7004t)
	OS := linux
	ARCH := arm
	TAGS := debug normal hi3520dv200
	MY_TOOL_CHAIN := arm-hisiv300-linux-
endif


# "linux", "windows", "darwin", "stm32", "js"
MY_OS := $(OS)

# "amd64", "386", "arm", "arm64", "wasm"
MY_ARCH := $(ARCH)

# go编译标签 -tags="debug hi3520dv200"
MY_GO_TAGS := -tags="$(TAGS)"

# gcc/cgo静态链接项
MY_GCC_STATIC := -static
MY_CGO_STATIC := -ldflags -extldflags=-static

# go main 目录
MY_GOPATH := $(PWD)/../../../../
MY_GO_MAIN := github.com/lishaoliang/klb/src/main


# gcc编译工具链
CC		:= $(MY_TOOL_CHAIN)gcc
CXX		:= $(MY_TOOL_CHAIN)g++
CAR		:= $(MY_TOOL_CHAIN)ar
CRANLIB	:= $(MY_TOOL_CHAIN)ranlib
CSTRIP	:= $(MY_TOOL_CHAIN)strip
MAKE	:= make
RM		:= -rm
RM_F	:= -rm -f
RM_RF	:= -rm -rf
CP		:= -cp
CP_F	:= -cp -f
CP_RF	:= -cp -rf


# 海思静态库; HISI_SDK_PATH(in), HISI_LIBS(out), HISI_INC(out)
ifeq ($(target), dopi3516dv3)
	HISI_SDK_PATH := $(PWD)/hi3516dv300
	-include ./hi3516dv300/Makefile.linux.param
	MY_MNP_DEV_LIB := -lmnp_dev_dopi_3516dv3
else ifeq ($(target), dopi16ev2)
	HISI_SDK_PATH := $(PWD)/hi3516ev200
	-include ./hi3516ev200/Makefile.linux.param
	MY_MNP_DEV_LIB := -lmnp_dev_dop16ev2
else ifeq ($(target), ahb7004t)
	HISI_SDK_PATH := $(PWD)/hi3520dv200
	-include ./hi3520dv200/Makefile.linux.param
	MY_MNP_DEV_LIB := -lmnp_dev_ahb7004t
endif


# 编译目标
MY_TARGET_NAME := klb
MY_TARGET_EXE := ./build/$(MY_TARGET_NAME)


# cgo 部分引用的C头文件目录
MY_CGO_CFLAGS := "
MY_CGO_CFLAGS += -I $(PWD)/../klb/inc -I $(PWD)/../klb/src_c/klua/lua-5.4.1/src
MY_CGO_CFLAGS += "

# cgo 部分需要链接的库, -Xlinker 库直接有交叉引用函数
MY_CGO_LDFLAGS := "
MY_CGO_LDFLAGS += -L $(PWD)/../klb/lib -lklb_c
MY_CGO_LDFLAGS += -L $(PWD)/lib $(MY_MNP_DEV_LIB)
MY_CGO_LDFLAGS += -Xlinker -lklb_c $(MY_MNP_DEV_LIB) -Xlinker
MY_CGO_LDFLAGS += $(HISI_LIBS)
MY_CGO_LDFLAGS += -lstdc++
MY_CGO_LDFLAGS += -lpthread -lrt -ldl -lm
MY_CGO_LDFLAGS += "

# cgo 前置环境
MY_CGO := CGO_ENABLED=1 CGO_CFLAGS=${MY_CGO_CFLAGS} CGO_LDFLAGS=${MY_CGO_LDFLAGS}
MY_CGO += GOOS=$(MY_OS) GOARCH=$(MY_ARCH) GOPATH=$(MY_GOPATH)
MY_CGO += CC=$(CC) CXX=$(CXX)


# 传递给子makefile的参数
MK_PARAMS := MY_TOOL_CHAIN=$(MY_TOOL_CHAIN) MY_CFLAGS_EX=-D__KLB_USE_KMNP_DEV_STATIC__


.PHONY: all clean strip upx cp

all: exe

exe: $(MY_TARGET_EXE)

$(MY_TARGET_EXE): depends
	$(my_tip)
	${MY_CGO} go build $(MY_GO_TAGS) $(MY_CGO_STATIC) -o $@ $(MY_GO_MAIN)

depends:
	if [ -f ../klb/src_c/Makefile ]; then $(MAKE) $(MK_PARAMS) -C ../klb/src_c; fi

ifeq ($(target), dopi3516dv3)
	if [ -f ./dopi_3516dv300/Makefile ]; then $(MAKE) $(MK_PARAMS) -C ./dopi_3516dv300; fi
else ifeq ($(target), dopi16ev2)
	if [ -f ./dopi16ev2/Makefile ]; then $(MAKE) $(MK_PARAMS) -C ./dopi16ev2; fi
else ifeq ($(target), ahb7004t)
	if [ -f ./ahb7004t/Makefile ]; then $(MAKE) $(MK_PARAMS) -C ./ahb7004t; fi
endif

clean:
	if [ -f ../klb/src_c/Makefile ]; then $(MAKE) -C ../klb/src_c clean; fi

ifeq ($(target), dopi3516dv3)
	if [ -f ./dopi_3516dv300/Makefile ]; then $(MAKE) $(MK_PARAMS) -C ./dopi_3516dv300 clean; fi
else ifeq ($(target), dopi16ev2)
	if [ -f ./dopi16ev2/Makefile ]; then $(MAKE) -C ./dopi16ev2 clean; fi
else ifeq ($(target), ahb7004t)
	if [ -f ./ahb7004t/Makefile ]; then $(MAKE) -C ./ahb7004t clean; fi
endif

	@echo "++++++ make clean ++++++"
	@echo "+ PWD = $(PWD)"
	@echo "++ RM_F = $(RM_F)"
	$(RM_F) $(MY_TARGET_EXE)
	@echo "+++++++++++++++++++++++++"

strip:
	$(CSTRIP) $(MY_TARGET_EXE)

upx:
	$(CSTRIP) $(MY_TARGET_EXE)
	./upx $(MY_TARGET_EXE)

cp:
	$(CP_RF) ../klb/lib/base ./build/
	$(CP_RF) ../klb/lib/html ./build/
	$(CP_RF) ../klb/lib/http ./build/
	$(CP_RF) ../klb/lib/rtsp ./build/
	$(CP_RF) ../klb/lib/server ./build/
	$(CP_RF) ../klb/lib/tls ./build/
	$(CP_RF) ../klb/lib/util ./build/
	$(CP_F)  ../klb/lib/conf.lua ./build/
	$(CP_F)  ../klb/lib/server.lua ./build/

define my_tip
	@echo "++++++ make tip ++++++"
	@echo "+ PWD = $(PWD)"
	@echo "+ MY_TOOL_CHAIN = $(MY_TOOL_CHAIN)"
	@echo "+ CC = $(CC)"
	@echo "+ CXX = $(CXX)"
	@echo "+ MY_GO_MAIN = $(MY_GO_MAIN)"
	@echo "+ HISI_SDK_PATH = $(HISI_SDK_PATH)"
	@echo "+ HISI_LIBS = $(HISI_LIBS)"
	@echo "+ MY_TARGET_EXE = $(MY_TARGET_EXE)"
	@echo "++++++++++++++++++++++"
endef
