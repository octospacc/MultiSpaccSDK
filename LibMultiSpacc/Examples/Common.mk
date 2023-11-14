AppName = $(notdir $(CURDIR))
AppAssets = ../CHARS.png
AppSources = $(wildcard *.c)
AppHeaders = $(wildcard *.h)
SpaccSources = $(wildcard ../../LibMultiSpacc/*.c)
SpaccHeaders = $(wildcard ../../LibMultiSpacc/*.h)
CFlags = -Os -Werror -Wpedantic -Wdeclaration-after-statement

# TODO: handle other unixes
ifeq ($(shell uname --operating-system), Msys)
	Host = Windows
else
	Host = Linux
endif

# When no user-specified target, build for the host system
ifndef Target
	ifeq ($(Host), Windows)
		Target = WindowsPC
	else ifeq ($(Host), Linux)
		Target = LinuxPC
	endif
endif

ifeq ($(Target), LinuxPC)
	ExeSuffix = .run
	Defines += -DTarget_LinuxPC -DMultiSpacc_Target_PC -DMultiSpacc_Target_Linux
	MultiSpacc_Target = SDL20

else ifeq ($(Target), WindowsPC)
	ExeSuffix = .exe
	Defines += -DTarget_WindowsPC -DMultiSpacc_Target_PC -DMultiSpacc_Target_Windows
	MultiSpacc_Target = SDL20
	ifneq ($(Host), Windows)
		ToolsSuffix = -mingw-w64
	endif

else ifeq ($(Target), Windows9x)
	ExeSuffix = .exe
	Defines += -DTarget_Windows9x -DMultiSpacc_Target_PC -DMultiSpacc_Target_Windows
	MultiSpacc_Target = SDL12
	LdFlags += -lmingw32 -static-libgcc
	ifeq ($(Host), Windows)
		ToolsSyspath = /c/Files/Sdk/mingw32/bin
		export PATH=$$PATH:$(ToolsSyspath)
	else
		ToolsSyspath = /opt/Sdk/mingw32/bin
		ToolsWrapper = wine
		LdFlags += -LZ:/opt/Sdk/mingw32/lib
	endif
	ToolsPrefix = $(ToolsSyspath)/

else ifeq ($(Target), Web)
	Defines += -DTarget_Web
	MultiSpacc_Target = Web

else ifeq ($(Target), NDS)
	Defines += -DTarget_NDS
	MultiSpacc_Target = NDS

else ifeq ($(Target), NES)
	Defines += -DTarget_NES
	MultiSpacc_Target = NES

endif

ifeq ($(MultiSpacc_Target), SDL12)
	Defines += -DMultiSpacc_Target_SDL12 -DMultiSpacc_Target_SDLCom -DMultiSpacc_Target_SDLCommon -DMultiSpacc_Target_SDLStandard
	CFlags += $(shell sdl-config --cflags)
	LdFlags += $(shell sdl-config --libs) -lSDLmain -lSDL -lSDL_image -lSDL_mixer -lSDL_ttf
	BuildProcess = __Normal__

else ifeq ($(MultiSpacc_Target), SDL20)
	Defines += -DMultiSpacc_Target_SDL20 -DMultiSpacc_Target_SDLCom -DMultiSpacc_Target_SDLCommon -DMultiSpacc_Target_SDLStandard
	CFlags += $(shell sdl2-config --cflags)
	LdFlags += $(shell sdl2-config --libs) -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf
	BuildProcess = __Normal__

else ifeq ($(MultiSpacc_Target), Web)
	Defines += -DMultiSpacc_Target_Web -DMultiSpacc_Target_SDL20 -DMultiSpacc_Target_SDLCom -DMultiSpacc_Target_SDLCommon -DMultiSpacc_Target_SDLWeb
	LdFlags += -sWASM=1 -sUSE_SDL=2 -sUSE_SDL_IMAGE=2 -sSDL2_IMAGE_FORMATS='["png"]' -sUSE_SDL_TTF=2 -sUSE_SDL_MIXER=2
	BuildProcess = __Web__

else ifeq ($(MultiSpacc_Target), NDS)
	ExeSuffix = .nds
	Defines += -DMultiSpacc_Target_NDS
	BuildProcess = __NDS__
	OutLauncher = melonDS

else ifeq ($(MultiSpacc_Target), NES)
	ExeSuffix = .nes
	Defines += -DMultiSpacc_Target_NES
	BuildProcess = __NES__
	OutLauncher = fceux

endif

BuildSources = $(AppSources) $(SpaccSources)
BuildObjects = $(BuildSources:.c=.o)

ifeq ($(BuildProcess), __Normal__)
	CC = $(ToolsWrapper) $(ToolsPrefix)gcc$(ToolsSuffix) $(CFlags) $(Defines)
endif

define BuildDirSourcesReformat
	for i in $(BuildDirSources)/*.c $(BuildDirSources)/*.h; do sed -i 's|#include[ \t]"../../LibMultiSpacc/|#include "LibMultiSpacc_|g' $$i; done
	for i in $(BuildDirSources)/*.c $(BuildDirSources)/*.h; do sed -i 's|#include[ \t]"../MultiSpacc|#include "LibMultiSpacc_MultiSpacc|g' $$i; done
	for i in $(BuildDirSources)/*.c $(BuildDirSources)/*.h; do sed -i 's|#include[ \t]"./|#include "LibMultiSpacc_|g' $$i; done
endef

All all: $(BuildProcess)

# TODO: use virtual build dirs even for normals to allow linking against different libraries without recleaning
__Normal__: $(BuildObjects)
	$(eval BuildDir = .)
	$(CC) $^ $(LdFlags) -o $(AppName)$(ExeSuffix)
	# TODO: copy required DLLs on PC for Dist

__Web__:
	$(eval BuildDir = ./Build/Web)
	mkdir -p $(BuildDir)
	emcc $(BuildSources) $(CFlags) $(Defines) $(LdFlags) --preload-file $(AppAssets)@CHARS.png -o $(BuildDir)/Emscripten.js
	cp ../Emscripten.html $(BuildDir)/$(AppName).html
	# TODO: bundle JS, WASM, and assets package in HTML file for Dist

# TODO: Fix include substitutions properly in non-standard build processes, and make all custom build processes lest wasteful

__NDS__:
	$(eval BuildDir = ./Build/NDS)
	$(eval BuildDirSources = $(BuildDir)/source)
	mkdir -p $(BuildDir)/source/.tmp
	cp ../NDS.mk $(BuildDir)/Makefile
	cp $(SpaccSources) $(SpaccHeaders) $(BuildDir)/source/.tmp/
	cd $(BuildDir)/source/.tmp; for i in *; do mv ./$$i ../LibMultiSpacc_$$i; done
	cp $(AppSources) $(AppHeaders) $(BuildDir)/source/
	$(BuildDirSourcesReformat)
	cd $(BuildDir); make

__NES__: __neslib__
	$(eval BuildDir = ./Build/NES)
	$(eval BuildDirSources = $(BuildDir))
	mkdir -p $(BuildDir)/.tmp
	cp $(SpaccSources) $(SpaccHeaders) $(BuildDir)/.tmp/
	cd $(BuildDir)/.tmp; for i in *; do mv ./$$i ../LibMultiSpacc_$$i; done
	cp $(AppSources) $(AppHeaders) $(BuildDir)/
	$(BuildDirSourcesReformat)
	cp ../../neslib/*.cfg ../../neslib/crt0.o ../../neslib/*.lib ../../neslib/*.h $(BuildDir)/
	printf ".segment \"CHARS\"\n\t.incbin \"CHARS.chr\"" > $(BuildDir)/CHARS.s
	echo "ProjectRoot=../..; AppName='$(AppName)'; AppAssets='$(AppAssets)'; Defines='$(Defines)';" > $(BuildDir)/Make.sh
	cat ../NES.mk.sh >> $(BuildDir)/Make.sh
	cd $(BuildDir); sh ./Make.sh

__neslib__:
	cd ../../neslib; make

Run run: All
	$(OutLauncher) $(BuildDir)/$(AppName)$(ExeSuffix)

Clean clean Clear clear:
	find -L . -name "*.o" -type f -delete
	find -L ../../LibMultiSpacc -name "*.o" -type f -delete
	rm -f ./$(AppName)$(ExeSuffix)
	rm -rf ./Build

.PHONY: All all Run run Clean clean Clear clear