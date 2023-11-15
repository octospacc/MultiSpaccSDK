AppName = $(notdir $(CURDIR))
AppAssets = ../CHARS.png
AppSources = $(wildcard *.c)
AppHeaders = $(wildcard *.h)
SpaccSources = $(wildcard ../../LibMultiSpacc/*.c)
SpaccHeaders = $(wildcard ../../LibMultiSpacc/*.h)
CFlags = -Os -Werror -Wpedantic -Wdeclaration-after-statement

# Defaults
BuildProcess = Normal

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

else ifeq ($(Target), Switch)
	Defines += -DTarget_Switch
	MultiSpacc_Target = Switch

else ifeq ($(Target), 3DS)
	Defines += -DTarget_3DS
	MultiSpacc_Target = 3DS

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

else ifeq ($(MultiSpacc_Target), SDL20)
	Defines += -DMultiSpacc_Target_SDL20 -DMultiSpacc_Target_SDLCom -DMultiSpacc_Target_SDLCommon -DMultiSpacc_Target_SDLStandard
	CFlags += $(shell sdl2-config --cflags)
	LdFlags += $(shell sdl2-config --libs) -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf

else ifeq ($(MultiSpacc_Target), Web)
	Defines += -DMultiSpacc_Target_Web -DMultiSpacc_Target_SDL20 -DMultiSpacc_Target_SDLCom -DMultiSpacc_Target_SDLCommon -DMultiSpacc_Target_SDLWeb
	LdFlags += -sWASM=1 -sUSE_SDL=2 -sUSE_SDL_IMAGE=2 -sSDL2_IMAGE_FORMATS='["png"]' -sUSE_SDL_TTF=2 -sUSE_SDL_MIXER=2
	BuildProcess = Web

else ifeq ($(MultiSpacc_Target), Switch)
	ExeSuffix = .nro
	Defines += -DMultiSpacc_Target_Switch -DMultiSpacc_Target_SDL20 -DMultiSpacc_Target_SDLCom -DMultiSpacc_Target_SDLCommon
	Libs += -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lpng -ljpeg -lwebp -lm -lz -lminizip -lbz2
	BuildProcess = Switch

else ifeq ($(MultiSpacc_Target), 3DS)
	ExeSuffix = .3dsx
	Defines += -DMultiSpacc_Target_3DS
	BuildProcess = 3DS
	OutLauncher = citra

else ifeq ($(MultiSpacc_Target), NDS)
	ExeSuffix = .nds
	Defines += -DMultiSpacc_Target_NDS
	BuildProcess = NDS
	OutLauncher = melonDS

else ifeq ($(MultiSpacc_Target), NES)
	ExeSuffix = .nes
	Defines += -DMultiSpacc_Target_NES
	BuildProcess = NES
	OutLauncher = fceux

endif

BuildSources = $(AppSources) $(SpaccSources)
BuildObjects = $(BuildSources:.c=.o)

ifeq ($(BuildProcess), Normal)
	CC = $(ToolsWrapper) $(ToolsPrefix)gcc$(ToolsSuffix) $(CFlags) $(Defines)
	BuildDir = .
else
	BuildDir = ./Build/$(BuildProcess)
endif

# TODO: Fix include substitutions properly in this, and make all custom build processes lest wasteful of operations
define PrepareTargetBuildDir
	mkdir -p $(BuildDir)/.tmp $(BuildDirSources)/.tmp

	echo > $(BuildDir)/Makefile
	echo "TARGET = $(AppName)" >> $(BuildDir)/Makefile
	echo "MultiSpacc_Defines = $(Defines)" >> $(BuildDir)/Makefile
	echo "MultiSpacc_LdFlags = $(LdFlags)" >> $(BuildDir)/Makefile
	echo "MultiSpacc_Libs = $(Libs)" >> $(BuildDir)/Makefile
	if [ -f ../$(BuildProcess).mk ]; then cat ../$(BuildProcess).mk >> $(BuildDir)/Makefile; fi

	cp $(SpaccSources) $(SpaccHeaders) $(BuildDirSources)/.tmp/
	cd $(BuildDirSources)/.tmp; for i in *; do mv ./$$i ../LibMultiSpacc_$$i; done
	cp $(AppSources) $(AppHeaders) $(BuildDirSources)/

	for i in $(BuildDirSources)/*.c $(BuildDirSources)/*.h; do sed -i 's|#include[ \t]"../../LibMultiSpacc/|#include "LibMultiSpacc_|g' $$i; done
	for i in $(BuildDirSources)/*.c $(BuildDirSources)/*.h; do sed -i 's|#include[ \t]"../MultiSpacc|#include "LibMultiSpacc_MultiSpacc|g' $$i; done
	for i in $(BuildDirSources)/*.c $(BuildDirSources)/*.h; do sed -i 's|#include[ \t]"./|#include "LibMultiSpacc_|g' $$i; done
endef

define TargetBuildAppToCpp
	#mv $(BuildDirSources)/$(AppName).c $(BuildDirSources)/$(AppName).cpp
	$(shell mv $(BuildDirSources)/MultiSpacc_* $(BuildDirSources)/.tmp/)
	for File in $(BuildDirSources)/*.c; do mv $${File} $${File}pp; done
	$(shell mv $(BuildDirSources)/.tmp/* $(BuildDirSources)/)
endef

All all: __$(BuildProcess)__

# TODO: use virtual build dirs even for normals to allow linking against different libraries without recleaning
__Normal__: $(BuildObjects)
	$(CC) $^ $(LdFlags) $(Libs) -o $(AppName)$(ExeSuffix)
	# TODO: copy required DLLs on PC for Dist

__Web__:
	mkdir -p $(BuildDir)
	emcc $(BuildSources) $(CFlags) $(Defines) $(LdFlags) --preload-file $(AppAssets)@CHARS.png -o $(BuildDir)/Emscripten.js
	cp ../Emscripten.html $(BuildDir)/$(AppName).html
	# TODO: bundle JS, WASM, and assets package in HTML file for Dist

__Switch__:
	$(eval BuildDirSources = $(BuildDir)/source)
	$(PrepareTargetBuildDir)
	$(TargetBuildAppToCpp)
	mkdir -p $(BuildDir)/romfs/.dummy
	touch $(BuildDir)/romfs/.dummy/.dummy
	cp $(AppAssets) $(BuildDir)/romfs/
	cd $(BuildDir); make

__3DS__ __NDS__:
	$(eval BuildDirSources = $(BuildDir)/source)
	$(PrepareTargetBuildDir)
	cd $(BuildDir); make

__NES__: __neslib__
	$(eval BuildDirSources = $(BuildDir))
	$(PrepareTargetBuildDir)
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