AppName = $(notdir ${CURDIR})
AppSources = $(wildcard *.c)
AppHeaders = $(wildcard *.h)
SpaccSources = $(wildcard ../../LibMultiSpacc/*.c)
SpaccHeaders = $(wildcard ../../LibMultiSpacc/*.h)
CFlags = -Os -Wpedantic -Werror

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
	BuildProcess = Normal
else ifeq ($(MultiSpacc_Target), SDL20)
	Defines += -DMultiSpacc_Target_SDL20 -DMultiSpacc_Target_SDLCom
	CFlags += $(shell sdl2-config --cflags)
	LdFlags += $(shell sdl2-config --libs) -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf
	BuildProcess = Normal
else ifeq ($(MultiSpacc_Target), NDS)
	Defines += -DMultiSpacc_Target_NDS
	BuildProcess = NDS
else ifeq ($(MultiSpacc_Target), NES)
	Defines += -DMultiSpacc_Target_NES
	BuildProcess = NES
endif

CC = $(ToolsPrefix)gcc $(CFlags) $(Defines)
BuildSources = $(AppSources) $(SpaccSources)
Objects = $(BuildSources:.c=.o)

All all: $(BuildProcess)

Normal: $(Objects)
	$(CC) $^ $(LdFlags) -o $(AppName)$(ExeSuffix)

NDS:
	$(eval VirtualBuildDir = ./Build/NDS)
	mkdir -p $(VirtualBuildDir)/source/.tmp
	cp ../NDS.mk $(VirtualBuildDir)/Makefile
	cp $(SpaccSources) $(SpaccHeaders) $(VirtualBuildDir)/source/.tmp/
	cd $(VirtualBuildDir)/source/.tmp; for i in *; do mv ./$$i ../LibMultiSpacc_$$i; done
	cp $(AppSources) $(AppHeaders) $(VirtualBuildDir)/source/
	for i in $(VirtualBuildDir)/source/*; do sed -i 's|#include[ \t]"../../LibMultiSpacc/|#include "LibMultiSpacc_|g' $$i; done
	for i in $(VirtualBuildDir)/source/*; do sed -i 's|#include[ \t]"../MultiSpacc|#include "LibMultiSpacc_MultiSpacc|g' $$i; done
	for i in $(VirtualBuildDir)/source/*; do sed -i 's|#include[ \t]"./|#include "./LibMultiSpacc_|g' $$i; done
	cd $(VirtualBuildDir); make

NES:
	#

Run run: All
	./$(AppName)$(ExeSuffix)

Clean clean Clear clear:
	find -L . -name "*.o" -type f -delete
	find -L ../../LibMultiSpacc -name "*.o" -type f -delete
	rm -f ./$(AppName)$(ExeSuffix) ./$(AppName).*$(ExeSuffix)
	rm -rf ./Build
