##说明：
#1.包含编译配置(系统或架构相关宏以及编译链接选项等配置)
#2.关于架构相关宏定义的说明：
#(1) ARCH_X86/ARCH_ARM/ARCH_AARCH64/ARCH_MIPS/ARCH_PPC表示不同的架构平台；
#(2) ARCH_X86_64=1表示X86架构64位，ARCH_X86_64=0表示X86架构32位；
#(3) HAVE_X86ASM表示开启X86汇编；HAVE_NEON表示采用NEON技术。

#检测系统
OS = $(shell uname)

#设置是否调试
ifeq ($(DEBUG), 0)
	DEBUG_FLAGS := -O3
else
	DEBUG_FLAGS := -g
endif

#########################################
############linux系统 ###################
ifeq ($(findstring Linux, $(OS)), Linux)
	CROSS 			?= 
	CC				:= $(CROSS)gcc -fPIC -DPIC
	CPP				:= $(CROSS)g++ -fPIC -DPIC
	LD				:= $(CROSS)ld
	AR				:= $(CROSS)ar
	ASM				:=	yasm -DPIC
	
###ARM32架构	
ifeq ($(platform), arm32)
	ARCH_DEF		:= -DARCH_X86=0 -DARCH_ARM=1 -DARCH_AARCH64=0 -DARCH_MIPS=0 
	arch			:= arm
	EXTRA_CFLAGS 	:= -march=armv7-a -marm $(ARCH_DEF)
	EXTRA_LFLAGS	:= -march=armv7-a -marm -pie -fPIE
	EXTRA_AFLAGS 	:= -march=armv7-a $(ARCH_DEF)
	EXTRA_LFLAGS_SO := -shared
	LIB_DIR			:= ./../../out/$(OS)_$(arch)_$(platform)
	OUT_DIR			:= ./../../bin/$(OS)_$(arch)_$(platform)
endif

###ARM64架构	
ifeq ($(platform), arm64)
	ARCH_DEF		:= -DARCH_X86=0 -DARCH_ARM=0 -DARCH_AARCH64=1 -DARCH_MIPS=0 
	arch			:= aarch64
	EXTRA_CFLAGS 	:= -march=armv8-a $(ARCH_DEF)
	EXTRA_LFLAGS 	:= -march=armv8-a  -pie -fPIE
	EXTRA_AFLAGS 	:= -march=armv8-a $(ARCH_DEF)
	EXTRA_LFLAGS_SO := -shared
	LIB_DIR			:= ./../../out/$(OS)_$(arch)_$(platform)
	OUT_DIR			:= ./../../bin/$(OS)_$(arch)_$(platform)
endif

###MIPS64架构	
ifeq ($(platform), mips64)
	ARCH_DEF		:= -DARCH_X86=0 -DARCH_ARM=0 -DARCH_AARCH64=0 -DARCH_MIPS=1
	arch			:= mips64
	ASM				:= $(CROSS)gcc
	EXTRA_CFLAGS 	:= -march=loongarch64  $(ARCH_DEF)
	EXTRA_LFLAGS 	:= -march=loongarch64  -pie -fPIE
	EXTRA_AFLAGS 	:= -march=loongarch64  $(ARCH_DEF)
	EXTRA_LFLAGS_SO := -shared
	LIB_DIR			:= ./../../out/$(OS)_$(arch)_$(platform)
	OUT_DIR			:= ./../../bin/$(OS)_$(arch)_$(platform)
endif

###X86_32架构	
ifeq ($(platform), x86_32)
	ARCH_DEF		:= -DARCH_X86=1 -DARCH_ARM=0 -DARCH_AARCH64=0 -DARCH_MIPS=0
	arch			:= x86
	EXTRA_CFLAGS 	:= -m32 $(ARCH_DEF)
	EXTRA_LFLAGS 	:= -m32
	EXTRA_LFLAGS_SO := -shared
	EXTRA_AFLAGS 	:= -m x86 -DHAVE_ALIGNED_STACK=1 -DARCH_X86_64=0 -DHAVE_CPUNOP=0 -f elf32 $(ARCH_DEF)
	LIB_DIR			:= ./../../out/$(OS)_$(arch)_$(platform)
	OUT_DIR			:= ./../../bin/$(OS)_$(arch)_$(platform)
endif


###X86_64架构	
ifeq ($(platform), x86_64)
	ARCH_DEF		:= -DARCH_X86=1 -DARCH_ARM=0 -DARCH_AARCH64=0 -DARCH_MIPS=0
	arch			:= x86
	EXTRA_CFLAGS 	:= -m64 $(ARCH_DEF)
	EXTRA_LFLAGS 	:= -m64 
	EXTRA_LFLAGS_SO := -shared
	EXTRA_AFLAGS 	:= -m amd64 -DHAVE_ALIGNED_STACK=1 -DARCH_X86_64=1 -DHAVE_CPUNOP=0 -f elf64 $(ARCH_DEF)
	LIB_DIR			:= ./../../out/$(OS)_$(arch)_$(platform)
	OUT_DIR			:= ./../../bin/$(OS)_$(arch)_$(platform)
endif

endif

#########################################
############MAC/IOS系统 ###################
ifeq ($(findstring Darwin, $(OS)), Darwin) 

###MAC平台
ifeq ($(target_plat), mac)
	CROSS 			?= 
	CC				:= $(CROSS)gcc -fPIC -DPIC
	CPP				:= $(CROSS)g++ -fPIC -DPIC
	AR				:= $(CROSS)ar
	ASM				:=	yasm -DPIC
	
##X86_32架构
ifeq ($(platform), x86_32)
	ARCH_DEF 		:= -DMAC32 -DARCH_X86=1 -DARCH_ARM=0 -DARCH_AARCH64=0 -DARCH_MIPS=0
	arch			:= x86
	EXTRA_CFLAGS	:= -m32 -arch i386 $(ARCH_DEF)
	EXTRA_LFLAGS	:= -m32 -dynamiclib -Wl, -dynamic  #-read_only_relocs
	EXTRA_AFLAGS	:= -f macho32 -m x86  -DHAVE_ALIGNED_STACK=1 -DARCH_X86_64=0 -DHAVE_CPUNOP=0 -DPREFIX $(ARCH_DEF)
	EXTRA_LFLAGS_SO := -dynamiclib -Wl, -dynamic
	LIB_DIR			:= ./../../out/$(OS)_$(arch)_$(platform)
	OUT_DIR			:= ./../../bin/$(OS)_$(arch)_$(platform)
endif

##X86_64架构
ifeq ($(platform), x86_64)
	ARCH_DEF 		:= -DMAC64 -DARCH_X86=1 -DARCH_ARM=0 -DARCH_AARCH64=0 -DARCH_MIPS=0
	arch			:= x86
	EXTRA_CFLAGS	:= -m64 $(ARCH_DEF)
	EXTRA_LFLAGS	:= -m64 
	EXTRA_LFLAGS_SO := -shared -dynamiclib -Wl, -dynamic
	EXTRA_AFLAGS	:= -f macho64 -m amd64 -DHAVE_ALIGNED_STACK=1 -DARCH_X86_64=1 -DHAVE_CPUNOP=0  -DPREFIX $(ARCH_DEF)
	LIB_DIR			:= ./../../out/$(OS)_$(arch)_$(platform)
	OUT_DIR			:= ./../../bin/$(OS)_$(arch)_$(platform)
endif

## Apple Silicon arm64架构
ifeq ($(platform), arm64)
	ARCH_DEF	 	:= -DARCH_X86=0 -DARCH_ARM=0 -DARCH_AARCH64=1 -DARCH_MIPS=0
	arch		 	:= aarch64
	EXTRA_CFLAGS 	:= -arch arm64 $(ARCH_DEF)
	EXTRA_LFLAGS 	:= -arch arm64 
	EXTRA_LFLAGS_SO	:= -shared
	EXTRA_AFLAGS 	:= -arch arm64 -DPREFIX $(ARCH_DEF)
	LIB_DIR		 	:= ./../../out/$(OS)_$(arch)_$(platform)
	OUT_DIR		 	:= ./../../bin/$(OS)_$(arch)_$(platform)
endif

endif

###IOS平台
ifeq ($(target_plat), ios)
	CROSS 			:= iphone
ifeq ($(platform), ios32)
	CC				:= xcrun -sdk $(CROSS)os clang
	CPP				:= g++
	AR				:= ar
	ASM				:= gas-preprocessor.pl -arch arm -as-type apple-clang --$(CC)
	
	ARCH_DEF		:= -DIOS32 -DARCH_X86=0 -DARCH_ARM=1 -DARCH_AARCH64=0 -DARCH_MIPS=0
	arch			:= arm
	EXTRA_CFLAGS	:= -arch armv7 -mios-version-min=6.0 $(ARCH_DEF)
	EXTRA_LFLAGS	:= -arch armv7 -mios-version-min=6.0 -shared
	EXTRA_AFLAGS	:= -arch armv7 -mios-version-min=6.0 -DPREFIX $(ARCH_DEF)
	LIB_DIR			:= ./../../out/$(OS)_$(arch)_$(platform)
	OUT_DIR			:= ./../../bin/$(OS)_$(arch)_$(platform)
endif

ifeq ($(platform), ios64)
	CC				:= xcrun -sdk $(CROSS)os clang
	CPP 			:= g++
	AR				:= ar
	ASM				:= gas-preprocessor.pl -arch aarch64 -as-type apple-clang --$(CC)
	
	ARCH_DEF		:= -DIOS64 -DARCH_X86=0 -DARCH_ARM=0 -DARCH_AARCH64=1 -DARCH_MIPS=0
	arch			:= aarch64
	EXTRA_CFLAGS 	:= -arch arm64 -mios-version-min=6.0 $(ARCH_DEF)
	EXTRA_LFLAGS 	:= -arch arm64 -mios-version-min=6.0 -shared
	EXTRA_AFLAGS 	:= -arch arm64 -mios-version-min=6.0 $(ARCH_DEF) -DPREFIX
	LIB_DIR			:= ./../../out/$(OS)_$(arch)_$(platform)
	OUT_DIR			:= ./../../bin/$(OS)_$(arch)_$(platform)
endif 

ifeq ($(platform), ios_sim32)
	CC				:= xcrun -sdk $(CROSS)simulator14.4 clang
	CPP				:= g++
	AR				:= ar
	ASM				:= yasm
	
	ARCH_DEF	 	:= -DARCH_X86=1 -DARCH_ARM=0 -DARCH_AARCH64=0 -DARCH_MIPS=0
	arch		 	:= x86
	EXTRA_CFLAGS 	:= -arch i386 -mios-simulator-version-min=6.0 $(ARCH_DEF)
	EXTRA_LFLAGS 	:= -arch i386 -mios-simulator-version-min=6.0 -shared -Wl, -Bsymbolic-functions -read_only_relocs suppress
	EXTRA_AFLAGS 	:= -f macho32 -m x86 $(ARCH_DEF)
	LIB_DIR			:= ./../../out/$(OS)_$(arch)_$(platform)
	OUT_DIR			:= ./../../bin/$(OS)_$(arch)_$(platform)
endif 

ifeq ($(platform), ios_sim64)
	CC				:= xcrun -sdk $(CROSS)simulator14.4 clang
	CPP				:= g++
	AR				:= ar
	ASM				:= yasm
	
	ARCH_DEF	 	:= -DARCH_X86=1 -DARCH_ARM=0 -DARCH_AARCH64=0 -DARCH_MIPS=0
	arch		 	:= x86
	EXTRA_CFLAGS 	:= -arch x86_64 -mios-simulator-version-min=6.0 $(ARCH_DEF)
	EXTRA_LFLAGS 	:= -arch x86_64 -mios-simulator-version-min=6.0 -shared
	EXTRA_AFLAGS 	:= -f macho64 -m amd64 $(ARCH_DEF)
	LIB_DIR			:= ./../../out/$(OS)_$(arch)_$(platform)
	OUT_DIR			:= ./../../bin/$(OS)_$(arch)_$(platform)
endif 

endif ##ifeq ($(target_plat), ios)

endif

######GIT版本获取#########
GIT=$(shell git rev-list -n 1 HEAD | cut -c 1-7)

######汇编宏的配置#######
ifeq ($(PUREC), 0)
ifeq ($(findstring Linux, $(OS)), Linux)
###X86_32架构	
ifeq ($(platform), x86_32)
EXTRA_CFLAGS += -DARCH_X86_64=0 -DARCH_ARM=0 -DARCH_AARCH64=0 -DHAVE_X86ASM=1 -DHAVE_SSE=1 -DHAVE_AVX=1
endif

###X86_64架构	
ifeq ($(platform), x86_64)
EXTRA_CFLAGS += -DARCH_X86_64=1 -DARCH_ARM=0 -DARCH_AARCH64=0 -DHAVE_X86ASM=1 -DHAVE_SSE=1 -DHAVE_AVX=1
endif

###ARM32架构	
ifeq ($(platform), arm32)
EXTRA_CFLAGS += -DARCH_ARM=1 -DHAVE_NEON=1
endif

###ARM64架构	
ifeq ($(platform), arm64)
EXTRA_CFLAGS += -DARCH_AARCH64=1 -DHAVE_NEON=1 -DHAVE_ARMV8=1 -DHAVE_VFP=1
endif

###MIPS64架构	
ifeq ($(platform), mips64)
EXTRA_CFLAGS += -DARCH_MIPS=1 -DHAVE_MSA=1
endif
endif

ifeq ($(findstring Darwin, $(OS)), Darwin) 

###MAC32架构	
ifeq ($(platform), x86_32)
EXTRA_CFLAGS += -DARCH_X86_64=0 -DARCH_ARM=0 -DARCH_AARCH64=0 -DHAVE_X86ASM=1 -DHAVE_SSE=1 -DHAVE_AVX=1
endif

###MAC64架构	
ifeq ($(platform), x86_64)
EXTRA_CFLAGS += -DARCH_X86_64=1 -DARCH_ARM=0 -DARCH_AARCH64=0 -DHAVE_X86ASM=1 -DHAVE_SSE=1 -DHAVE_AVX=1
endif

###ARM64架构(Apple Silicon)	
ifeq ($(platform), arm64)
EXTRA_CFLAGS += -DARCH_AARCH64=1 -DHAVE_NEON=1 -DHAVE_ARMV8=1 -DHAVE_VFP=1
endif

###IOS平台
ifeq ($(target_plat), ios)
ifeq ($(platform), ios32)
EXTRA_CFLAGS += -DARCH_ARM=1 -DHAVE_NEON=1 -DHAVE_ARMV8=0 -DHAVE_VFP=0
endif
ifeq ($(platform), ios64)
EXTRA_CFLAGS += -DARCH_AARCH64=1 -DHAVE_NEON=1 -DHAVE_ARMV8=0 -DHAVE_VFP=0
endif

endif
endif
endif
