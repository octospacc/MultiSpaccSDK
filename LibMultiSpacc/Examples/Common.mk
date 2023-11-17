AppName = $(notdir $(CURDIR))
AppAssets = ../CHARS.xcf
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
	Defines += -DTarget_Windows9x -DMultiSpacc_Target_Windows9x -DMultiSpacc_Target_PC -DMultiSpacc_Target_Windows
	MultiSpacc_Target = SDL12
	LdFlags += -lmingw32 -static-libgcc
	ifeq ($(Host), Windows)
		ToolsSyspath = /c/Files/Sdk/mingw32/bin
		# TODO: Find a workaround to the path hardcoding here
		export PATH=/usr/bin:/c/Files/Applications:$(ToolsSyspath)
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

ConstSdlDefines = -DMultiSpacc_Target_SDLCom -DMultiSpacc_Target_SDLCommon -DMultiSpacc_Backend_SDLCommon
ConstSdl12Defines = $(ConstSdlDefines) -DMultiSpacc_Target_SDL12 -DMultiSpacc_Backend_SDL12
ConstSdl20Defines = $(ConstSdlDefines) -DMultiSpacc_Target_SDL20 -DMultiSpacc_Backend_SDL20

ifeq ($(MultiSpacc_Target), SDL12)
	Defines +=  $(ConstSdl12Defines) -DMultiSpacc_Target_SDLStandard
	CFlags += $(shell sdl-config --cflags)
	LdFlags += $(shell sdl-config --libs) -lSDLmain -lSDL -lSDL_image -lSDL_mixer -lSDL_ttf

else ifeq ($(MultiSpacc_Target), SDL20)
	Defines += $(ConstSdl20Defines) -DMultiSpacc_Target_SDLStandard
	CFlags += $(shell sdl2-config --cflags)
	LdFlags += $(shell sdl2-config --libs) -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf

else ifeq ($(MultiSpacc_Target), Web)
	Defines += -DMultiSpacc_Target_Web $(ConstSdl20Defines) -DMultiSpacc_Target_SDLWeb
	LdFlags += -sWASM=1 -sUSE_SDL=2 -sUSE_SDL_IMAGE=2 -sSDL2_IMAGE_FORMATS='["png"]' -sUSE_SDL_TTF=2 -sUSE_SDL_MIXER=2
	BuildProcess = Web

else ifeq ($(MultiSpacc_Target), Switch)
	ExeSuffix = .nro
	Defines += -DMultiSpacc_Target_Switch $(ConstSdl20Defines)
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

# For C++ build systems, main() must be in a .cpp file, to be compiled by the CXX
define TargetBuildAppToCpp
	$(shell mv $(BuildDirSources)/MultiSpacc_* $(BuildDirSources)/.tmp/)
	for File in $(BuildDirSources)/*.c; do mv $${File} $${File}pp; done
	$(shell mv $(BuildDirSources)/.tmp/* $(BuildDirSources)/)
endef

All all: __Assets__ __$(BuildProcess)__

__Assets__:
	mkdir -p ./Build/Assets
	for SrcFile in $(AppAssets); \
	do \
		SrcExt="$${SrcFile##*.}"; \
		DstName="$${SrcFile##*/}"; \
		DstName="$${DstName%.*}"; \
		if [ "$${SrcExt}" = xcf ]; then \
			magick convert "$${SrcFile}" "./Build/Assets/$${DstName}.png"; \
			mv "./Build/Assets/$${DstName}-0.png" "./Build/Assets/$${DstName}.4.png"; \
			mv "./Build/Assets/$${DstName}-1.png" "./Build/Assets/$${DstName}.png"; \
		fi; \
	done

# TODO: use virtual build dirs even for normals to allow linking against different libraries without recleaning
# TODO: copy required DLLs on PC for Dist
__Normal__: $(BuildObjects)
	$(CC) $^ $(LdFlags) $(Libs) -o $(AppName)$(ExeSuffix)

# TODO: bundle JS, WASM, and assets package in HTML file for Dist
# TODO: remove asset hardcoding from here
__Web__:
	mkdir -p $(BuildDir)
	emcc $(BuildSources) $(CFlags) $(Defines) $(LdFlags) --preload-file $(AppAssets)@CHARS.png -o $(BuildDir)/Emscripten.js
	cp ../Emscripten.html $(BuildDir)/$(AppName).html

__Switch__:
	$(eval BuildDirSources = $(BuildDir)/source)
	$(PrepareTargetBuildDir)
	$(TargetBuildAppToCpp)
	#mkdir -p $(BuildDir)/romfs/.dummy
	#touch $(BuildDir)/romfs/.dummy/.dummy
	mkdir -p $(BuildDir)/romfs
	cp -r ./Build/Assets $(BuildDir)/romfs/Assets
	cd $(BuildDir); make -j$$(nproc --all)

__3DS__ __NDS__:
	$(eval BuildDirSources = $(BuildDir)/source)
	$(PrepareTargetBuildDir)
	cd $(BuildDir); make -j$$(nproc --all)

__NES__: __neslib__
	$(eval BuildDirSources = $(BuildDir))
	$(PrepareTargetBuildDir)
	cp ../../neslib/*.cfg ../../neslib/crt0.o ../../neslib/*.lib ../../neslib/*.h $(BuildDir)/
	printf ".segment \"CHARS\"\n\t.incbin \"CHARS.chr\"" > $(BuildDir)/CHARS.s
	echo "ProjectRoot=../..; AppName='$(AppName)'; AppAssets='$(AppAssets)'; Defines='$(Defines)';" > $(BuildDir)/Make.sh
	cat ../NES.mk.sh >> $(BuildDir)/Make.sh
	cd $(BuildDir); sh ./Make.sh

__neslib__:
	cd ../../neslib; make -j$$(nproc --all)

Run run: All
	cd ./Build; $(OutLauncher) ../$(BuildDir)/$(AppName)$(ExeSuffix); cd ..

Clean clean Clear clear:
	find -L . -name "*.o" -type f -delete
	find -L ../../LibMultiSpacc -name "*.o" -type f -delete
	rm -f ./$(AppName)$(ExeSuffix)
	rm -rf ./Build

.PHONY: All all Run run Clean clean Clear clear