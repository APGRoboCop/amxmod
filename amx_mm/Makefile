# Console command(s) for compiling on Cygwin/MinGW:
#   Example: make ARCH=amd64
#      ARCH: by default compiles for i686 for Linux, i386 for Win32
#      JITORNOT: by default this is 1, so if you want JIT disabled you must declare JITORNOT=0

MODNAME = amx_mm
SRCFILES = meta_api.cpp CFile.cpp CString.cpp CVault.cpp vault.cpp\
float.cpp file.cpp modules.cpp CMisc.cpp CTask.cpp string.cpp\
amxmod.cpp CEvent.cpp CCmd.cpp CLogEvent.cpp srvcmd.cpp strptime.cpp\
CForward.cpp CPlugin.cpp CModule.cpp CMenu.cpp emsg.cpp message.cpp util.cpp vector.cpp\
CTranslator.cpp math.cpp amx.cpp amxcore.cpp amxtime.cpp power.cpp format.cpp \
optimizer.cpp
CSRCFILES = 
ASRCFILES = amxjitsn.asm helpers-x86.asm

EXTRA_LIBS_LINUX = 
EXTRA_LIBS_WIN32 = 
EXTRA_LIBDIRS_LINUX = -Lextra/lib_linux
EXTRA_LIBDIRS_WIN32 = -Lextra/lib_win32

EXTRA_INCLUDEDIRS = -Iextra/include

EXTRA_FLAGS = -Dstrcmpi=strcasecmp

SDKTOP=../../hlsdk-2.3-p4
SDKSRC=$(SDKTOP)/multiplayer
METADIR=../../metamod-p/metamod
SRCDIR=.

OBJDIR_LINUX=obj.linux
OBJDIR_WIN32=obj.win32

ifeq "$(shell uname | cut -d _ -f1)" "CYGWIN"
	HOST=cygwin
endif

ifdef ComSpec
	ifeq "$(HOST)" "cygwin"
		OS=LINUX
		PATH_WIN=/usr/local/cross-tools/i386-mingw32msvc/bin
		CLEAN=clean_linux
		MCPU=-mcpu
	else
		OS=WIN32
		PATH_WIN=/mingw/bin
		CLEAN=clean_win32
		MCPU=-mtune
	endif
else
	ifdef COMSPEC
		ifeq "$(HOST)" "cygwin"
			OS=LINUX
			PATH_WIN=/usr/local/cross-tools/i386-mingw32msvc/bin
			CLEAN=clean_linux
			MCPU=-mcpu
		else
			OS=WIN32
			PATH_WIN=/mingw/bin
			CLEAN=clean_win32
			MCPU=-mtune
		endif
	else
		OS=LINUX
		PATH_WIN=/usr/local/cross-tools/i386-mingw32msvc/bin
		CLEAN=clean_linux
		MCPU=-mcpu
	endif
endif

ifeq "$(HOST)" "cygwin"
	ifeq "$(ARCH)" "amd64"
		CC_LINUX=gcc-linux-x86_64
		CXX_LINUX=g++-linux-x86_64
	else
		CC_LINUX=gcc-linux
		CXX_LINUX=g++-linux
	endif
else
	ifeq "$(ARCH)" "amd64"
		CC_LINUX=gcc-x86_64
		CXX_LINUX=g++-x86_64
	else
		CC_LINUX=gcc
		CXX_LINUX=g++
	endif
endif

ifeq "$(OS)" "LINUX"
	INSTALL=install -m 644
	LD_WINDLL= PATH=$(PATH_WIN) $(PATH_WIN)/dllwrap
	DEFAULT=linux
	NASM=nasm
else	# WIN32
	INSTALL=cp
	CC_WIN32=i686-w64-mingw32-gcc
	CXX_WIN32=i686-w64-mingw32-g++
	LD_WINDLL= i686-w64-mingw32-dllwrap
	DEFAULT=win32
	NASM=mingw-w64-i686-nasm
endif

ifeq "$(CHINESE)" "1"
	DOCHINESE=-DCHINESE_SUPPORT
else
	DOCHINESE=
endif

# Just call everything i386 unless its a 64bit binary!
ifeq "$(ARCH)" "amd64"
	LIBFILE_LINUX = $(MODNAME)_amd64.so
else
	LIBFILE_LINUX = $(MODNAME)_i386.so
endif

LIBFILE_WIN32 = $(MODNAME).dll
TARGET_LINUX = $(OBJDIR_LINUX)/$(LIBFILE_LINUX)
TARGET_WIN32 = $(OBJDIR_WIN32)/$(LIBFILE_WIN32)

FILES_ALL = *.cpp *.h [A-Z]* *.rc
ifeq "$(OS)" "LINUX"
#	ASRCFILES := $(shell ls -t $(SRCFILES))
else
#	ASRCFILES := $(shell dir /b)
endif

OBJ_LINUX := $(SRCFILES:%.cpp=$(OBJDIR_LINUX)/%.o)
OBJC_LINUX := $(CSRCFILES:%.c=$(OBJDIR_LINUX)/%.o)
OBJ_WIN32 := $(SRCFILES:%.cpp=$(OBJDIR_WIN32)/%.o)
OBJC_WIN32 := $(CSRCFILES:%.c=$(OBJDIR_WIN32)/%.o)

OBJA_LINUX = $(ASRCFILES:%.asm=$(OBJDIR_LINUX)/%.o)
OBJA_WIN32 = $(ASRCFILES:%.asm=$(OBJDIR_WIN32)/%.o)

# architecture tuning by arch type
ifeq "$(ARCH)" "amd64"
	CCOPT_ARCH = -DHAVE_I64 -DSMALL_CELL_SIZE=64
else
	# pentium or newer
	CCOPT_ARCH = -march=i686 -mtune=generic
endif

# JIT can be:
# - disabled (JIT!=1 and JITORNOT!=1)
# - optionally enabled (JIT!=1 and JITORNOT=1) (users can choose)
# - enabled (JIT=1)

# (you can also comment this line and pass what you want on your 'make' command line: "make JITORNOT=1")

ifeq "$(JIT)" "1"
	DOJIT=-DJIT
else
	ifeq "$(JITORNOT)" "0"
		DOJIT=
		OBJA_LINUX=
		OBJA_WIN32=
		ASRCFILES=
	else
	 DOJIT=-DJITORNOT
	endif
endif

# optimisation level; overridden for certain problematic files
CCO = -O2 -fomit-frame-pointer -fno-strict-aliasing -pipe -mmmx -msse -msse2 -mfpmath=sse

CCOPT = $(CCO) $(CCOPT_ARCH) -fno-exceptions -fno-rtti -DNDEBUG $(DOJIT) $(DOCHINESE)

INCLUDEDIRS=-I../curl/include -I$(SRCDIR) -I$(METADIR) -I$(SDKSRC)/engine -I$(SDKSRC)/common -I$(SDKSRC)/pm_shared -I$(SDKSRC)/dlls -I$(SDKSRC) $(EXTRA_INCLUDEDIRS)
ODEF=-DOPT_TYPE=\"optimized\"
CFLAGS=-Wall -Wno-unknown-pragmas -Wno-deprecated -Wno-write-strings -Wno-attributes
CFLAGS:=$(CCOPT) $(CFLAGS) $(ODEF) $(EXTRA_FLAGS)

DO_CC_LINUX=$(CC_LINUX) $(CFLAGS) -fPIC $(INCLUDEDIRS) -DLINUX -o $@ -c $<
DO_CXX_LINUX=$(CXX_LINUX) $(CFLAGS) -fPIC $(INCLUDEDIRS) -DLINUX -o $@ -c $<
DO_ASM_LINUX=$(NASM) -O1 -o $@ -f elf $<

DO_CC_WIN32=$(CC_WIN32) $(CFLAGS) $(INCLUDEDIRS) -DWIN32 -D_WIN32 -o $@ -c $<
DO_CXX_WIN32=$(CXX_WIN32) $(CFLAGS) $(INCLUDEDIRS) -DWIN32 -D_WIN32 -o $@ -c $<
DO_ASM_WIN32=$(NASM) -O1 -o $@ -f win32 $<

LINK_LINUX=$(CC_LINUX) $(CFLAGS) -shared -ldl -lm -static-libgcc -static-libstdc++ $(OBJ_LINUX) $(OBJC_LINUX) $(OBJA_LINUX) $(EXTRA_LIBDIRS_LINUX) $(EXTRA_LIBS_LINUX) -s -o $@ 
LINK_WIN32=$(CC_WIN32) -mdll -Xlinker --add-stdcall-alias $(OBJ_WIN32) $(OBJC_WIN32) $(OBJA_WIN32) $(EXTRA_LIBDIRS_WIN32) $(EXTRA_LIBS_WIN32) -s -o $@

$(OBJDIR_LINUX)/%.o: $(SRCDIR)/%.c
	$(DO_CC_LINUX)

$(OBJDIR_LINUX)/%.o: $(SRCDIR)/%.cpp
	$(DO_CXX_LINUX)

$(OBJDIR_WIN32)/%.o: $(SRCDIR)/%.c
	$(DO_CC_WIN32)

$(OBJDIR_WIN32)/%.o: $(SRCDIR)/%.cpp
	$(DO_CXX_WIN32)

$(OBJDIR_LINUX)/%.o: $(SRCDIR)/%.asm
	$(DO_ASM_LINUX)

$(OBJDIR_WIN32)/%.o: $(SRCDIR)/%.asm
	$(DO_ASM_WIN32)

default: $(DEFAULT)

$(TARGET_LINUX): $(OBJDIR_LINUX) $(OBJ_LINUX) $(OBJC_LINUX) $(OBJA_LINUX)
	$(LINK_LINUX)

$(TARGET_WIN32): $(OBJDIR_WIN32) $(OBJ_WIN32) $(OBJC_WIN32) $(OBJA_WIN32)
	$(LINK_WIN32)

$(OBJDIR_LINUX):
	mkdir $@

$(OBJDIR_WIN32):
	mkdir $@

win32: $(TARGET_WIN32)

linux: $(TARGET_LINUX)

clean: $(CLEAN)

clean_linux:
	-rm -f $(OBJDIR_LINUX)/*

clean_win32:
	-rm -f $(OBJDIR_WIN32)/*

clean_both: clean_linux clean_win32
