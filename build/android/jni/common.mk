##主要指定源文件路径和头文件路径
SCRIPT_PATH := $(call my-dir)

SRC_DIR := $(SCRIPT_PATH)/../../../src

##头文件路径
LOCAL_C_INCLUDES := ./	\
			$(SRC_DIR)		\
			$(SRC_DIR)/../include \
			$(SRC_DIR)/$(INCLUDEASM)

##纯C文件路径
C_SRCS := $(wildcard $(SRC_DIR)/*.c) 
C_SRCS += $(wildcard $(SRC_DIR)/../utils/*.c) 

##汇编文件路径
A_SRCS := 

ifeq ($(PURE_C),0)

#x86架构
#ifeq ($(findstring x86,$(TARGET_ARCH_ABI)),x86)
#IFLAGS += -I$(SRC_DIR)/x86
#A_SRCS		+= $(wildcard $(SRC_DIR)/x86/*.asm) 
#A_SRCS		+= $(wildcard $(SRC_DIR)/../utils/x86/*.asm) 
#endif
ifeq ($(TARGET_ARCH_ABI), x86)
IFLAGS += -I$(SRC_DIR)/x86
A_SRCS		+= $(wildcard $(SRC_DIR)/x86/transpose_x86_32.asm) 
A_SRCS		+= $(wildcard $(SRC_DIR)/../utils/x86/*.asm) 
endif

ifeq ($(TARGET_ARCH_ABI), x86_64)
IFLAGS += -I$(SRC_DIR)/x86
A_SRCS		+= $(wildcard $(SRC_DIR)/x86/transpose_x86_64.asm) 
A_SRCS		+= $(wildcard $(SRC_DIR)/../utils/x86/*.asm) 
endif

#arm架构
#arm架构32位
ifeq ($(findstring armeabi, $(TARGET_ARCH_ABI)), armeabi)
IFLAGS += -I$(SRC_DIR)/arm
C_SRCS += $(wildcard $(SRC_DIR)/$(INCLUDEASM)/*.c)
A_SRCS += $(wildcard $(SRC_DIR)/arm/*.S) 
C_SRCS += $(wildcard $(SRC_DIR)/../utils/$(INCLUDEASM)/*.c)
A_SRCS += $(wildcard $(SRC_DIR)/../utils/arm/*.S) 
endif

#arm架构64位
ifeq ($(findstring arm64-v8a, $(TARGET_ARCH_ABI)), arm64-v8a)
IFLAGS += -I$(SRC_DIR)/aarch64
C_SRCS += $(wildcard $(SRC_DIR)/$(INCLUDEASM)/*.c)
A_SRCS += $(wildcard $(SRC_DIR)/aarch64/*.S) 
C_SRCS += $(wildcard $(SRC_DIR)/../utils/$(INCLUDEASM)/*.c)
A_SRCS += $(wildcard $(SRC_DIR)/../utils/aarch64/*.S) 
endif
endif

#$(warning $(CFLAGS)) 

##设置汇编文件
LOCAL_SRC_FILES := $(A_SRCS) $(C_SRCS)

