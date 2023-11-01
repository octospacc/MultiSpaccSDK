AppName = $(notdir ${CURDIR})
AppSources = $(wildcard *.c)
AppHeaders = $(wildcard *.h)
SpaccSources = $(wildcard ../../LibMultiSpacc/*.c)
SpaccHeaders = $(wildcard ../../LibMultiSpacc/*.h)
CFlags = -Os -Werror -Wpedantic -Wdeclaration-after-statement

# Default build is always for the host system
ifndef Target
	ifeq ($(shell uname --operating-system), Msys)
		Target = WindowsPC
	else
		Target = LinuxPC
	endif
endif

ifdef Target
	ifeq ($(Target), LinuxPC)
		ExeSuffix = .run
		Defines += -DTarget_LinuxPC
		MultiSpacc_Target = SDL20
	else ifeq ($(Target), WindowsPC)
		ExeSuffix = .exe
		Defines += -DTarget_WindowsPC
		MultiSpacc_Target = SDL20
	else ifeq ($(Target), NDS)
		Defines += -DTarget_NDS
		MultiSpacc_Target = NDS
	else ifeq ($(Target), NES)
		Defines += -DTarget_NES
		MultiSpacc_Target = NES
	endif
endif

ifeq ($(MultiSpacc_Target), SDL12)
	Defines += -DMultiSpacc_Target_SDL12 -DMultiSpacc_Target_SDLCom
	CFlags += $(shell sdl-config --cflags)
	LdFlags += $(shell sdl-config --libs) -lSDL -lSDL_image -lSDL_mixer -lSDL_ttf
	BuildProcess = __Normal__
else ifeq ($(MultiSpacc_Target), SDL20)
	Defines += -DMultiSpacc_Target_SDL20 -DMultiSpacc_Target_SDLCom
	CFlags += $(shell sdl2-config --cflags)
	LdFlags += $(shell sdl2-config --libs) -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf
	BuildProcess = __Normal__
else ifeq ($(MultiSpacc_Target), NDS)
	Defines += -DMultiSpacc_Target_NDS
	BuildProcess = __NDS__
else ifeq ($(MultiSpacc_Target), NES)
	Defines += -DMultiSpacc_Target_NES
	BuildProcess = __NES__
endif

CC = $(ToolsPrefix)gcc $(CFlags) $(Defines)
BuildSources = $(AppSources) $(SpaccSources)
BuildObjects = $(BuildSources:.c=.o)

All all: $(BuildProcess)

# TODO: use virtual build dirs even for normals to allow linking against different wrapped libraries
__Normal__: $(BuildObjects)
	$(CC) $^ $(LdFlags) -o $(AppName)$(ExeSuffix)

# TODO: Fix include substitutions properly in non-standard build processes

__NDS__:
	$(eval VirtualBuildDir = ./Build/NDS)
	mkdir -p $(VirtualBuildDir)/source/.tmp
	cp ../NDS.mk $(VirtualBuildDir)/Makefile
	cp $(SpaccSources) $(SpaccHeaders) $(VirtualBuildDir)/source/.tmp/
	cd $(VirtualBuildDir)/source/.tmp; for i in *; do mv ./$$i ../LibMultiSpacc_$$i; done
	cp $(AppSources) $(AppHeaders) $(VirtualBuildDir)/source/
	for i in $(VirtualBuildDir)/source/*.c $(VirtualBuildDir)/source/*.h; do sed -i 's|#include[ \t]"../../LibMultiSpacc/|#include "LibMultiSpacc_|g' $$i; done
	for i in $(VirtualBuildDir)/source/*.c $(VirtualBuildDir)/source/*.h; do sed -i 's|#include[ \t]"../MultiSpacc|#include "LibMultiSpacc_MultiSpacc|g' $$i; done
	for i in $(VirtualBuildDir)/source/*.c $(VirtualBuildDir)/source/*.h; do sed -i 's|#include[ \t]"./|#include "LibMultiSpacc_|g' $$i; done
	cd $(VirtualBuildDir); make

__NES__:
	cd ../../neslib; make
	$(eval VirtualBuildDir = ./Build/NES)
	mkdir -p $(VirtualBuildDir)/.tmp
	cp $(SpaccSources) $(SpaccHeaders) $(VirtualBuildDir)/.tmp/
	cd $(VirtualBuildDir)/.tmp; for i in *; do mv ./$$i ../LibMultiSpacc_$$i; done
	cp $(AppSources) $(AppHeaders) $(VirtualBuildDir)/
	for i in $(VirtualBuildDir)/*.c $(VirtualBuildDir)/*.h; do sed -i 's|#include[ \t]"../../LibMultiSpacc/|#include "LibMultiSpacc_|g' $$i; done
	for i in $(VirtualBuildDir)/*.c $(VirtualBuildDir)/*.h; do sed -i 's|#include[ \t]"../MultiSpacc|#include "LibMultiSpacc_MultiSpacc|g' $$i; done
	for i in $(VirtualBuildDir)/*.c $(VirtualBuildDir)/*.h; do sed -i 's|#include[ \t]"./|#include "LibMultiSpacc_|g' $$i; done
	cp ../../neslib/*.cfg ../../neslib/crt0.o ../../neslib/*.lib ../../neslib/*.h $(VirtualBuildDir)/
	printf ".segment \"CHARS\"\n\t.incbin \"CHARS.chr\"" > $(VirtualBuildDir)/CHARS.s
	echo "AppName='$(AppName)'; Defines='$(Defines)'; ProjectRoot=../..;" > $(VirtualBuildDir)/Make.sh
	cat ../NES.mk.sh >> $(VirtualBuildDir)/Make.sh
	cd $(VirtualBuildDir); sh ./Make.sh

Run run: All
	./$(AppName)$(ExeSuffix)

Clean clean Clear clear:
	find -L . -name "*.o" -type f -delete
	find -L ../../LibMultiSpacc -name "*.o" -type f -delete
	rm -f ./$(AppName)$(ExeSuffix)
	rm -rf ./Build
