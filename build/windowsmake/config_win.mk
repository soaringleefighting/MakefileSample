OS = $(shell uname)
$(warning $(OS))

ifeq ($(findstring MINGW, $(OS)), MINGW)  ## MINGW环境下运行windows makefile
CC	:= cl
CPP	:= cl
AR	:= lib
LD	:= link
ASM	:= yasm
RC	:= rc.exe

ifeq ($(platform), win32)
	EXTRA_CFLAGS := -DWIN32 -DARCH_X86_32 -O2
	EXTRA_SFLAGS += -f win32 -DPREFIX
	EXTRA_RCFLAGS := -DWIN32
	EXTRA_LDFLAGS :=
	arch := x86
	LIB_DIR		:= ./../../out/windows_$(arch)_$(platform)
	OUT_DIR		:= ./../../bin/windows_$(arch)_$(platform)
endif
ifeq ($(platform), x64)
	EXTRA_CFLAGS := -DWIN64 -DARCH_X86_64 -O2
	EXTRA_SFLAGS += -f x64 -DPREFIX
	EXTRA_RCFLAGS := -DWIN64
	EXTRA_LDFLAGS :=
	arch := x86
	LIB_DIR		:= ./../../out/windows_$(arch)_$(platform)
	OUT_DIR		:= ./../../bin/windows_$(arch)_$(platform)	
endif
endif

ifeq ($(findstring Linux, $(OS)), Linux)  ##LInux环境下运行Windows make，需要借助GNUWin32中的make
	CC	:= cl.exe
	LD	:= link.exe
	AR	:= lib.exe
	RC	:= rc.exe
	
ifeq ($(strip $(platform)), win32)
	EXTRA_CFLAGS := -W3 -nologo -DWIN32
	EXTRA_CFLAGS += -D_CRT_SECURE_NO_WARNINGS -O2
	EXTRA_SFLAGS += -f win32 -DPREFIX
	EXTRA_LDFLAGS :=
	EXTRA_RCFLAGS := -DWIN32
	arch := x86
	LIB_DIR		:= ./../../out/windows_$(arch)_$(platform)
	OUT_DIR		:= ./../../bin/windows_$(arch)_$(platform)		
endif

ifeq ($(strip $(platform)), x64)
	EXTRA_CFLAGS := -W3 -nologo -DWIN64
	EXTRA_CFLAGS += -D_CRT_SECURE_NO_WARNINGS -O2
	EXTRA_SFLAGS += -f x64 -DPREFIX
	EXTRA_LDFLAGS :=
	EXTRA_RCFLAGS := -DWIN64
	arch := x86
	LIB_DIR		:= ./../../out/windows_$(arch)_$(platform)
	OUT_DIR		:= ./../../bin/windows_$(arch)_$(platform)	
endif
endif