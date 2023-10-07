AppName = $(notdir ${CURDIR})
Sources = $(wildcard *.c ../../LibMultiSpacc/*.c)
CFlags = -O2 -Wpedantic -Werror

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
	endif
endif

ifeq ($(MultiSpacc_Target), SDL12)
	Defines += -DMultiSpacc_Target_SDL12
	CFlags += $(shell sdl-config --cflags)
	LdFlags += $(shell sdl-config --libs) -lSDL -lSDL_image -lSDL_mixer -lSDL_ttf
	Sources += $(wildcard ../../LibMultiSpacc/SDLCom/*.c ../../LibMultiSpacc/SDL12/*.c)
else ifeq ($(MultiSpacc_Target), SDL20)
	Defines += -DMultiSpacc_Target_SDL20
	CFlags += $(shell sdl2-config --cflags)
	LdFlags += $(shell sdl2-config --libs) -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf
	Sources += $(wildcard ../../LibMultiSpacc/SDLCom/*.c ../../LibMultiSpacc/SDL20/*.c)
else ifeq ($(MultiSpacc_Target), NDS)
	#include $(DEVKITARM)/ds_rules
	Defines += -DMultiSpacc_Target_NDS
	CFlags += -I$(DEVKITPRO)/libnds/include -DARM9
	Sources += $(wildcard ../../LibMultiSpacc/NDS/*.c)
	ToolsPrefix = $(DEVKITARM)/bin/arm-none-eabi-
endif

CC = $(ToolsPrefix)gcc $(CFlags) $(Defines)
Objects = $(Sources:.c=.o)

All all: $(AppName)

$(AppName): $(Objects)
	$(CC) $^ $(LdFlags) -o $(AppName)$(ExeSuffix)

Run run: All
	./$(AppName)$(ExeSuffix)

Clean clean:
	find -L . -name "*.o" -type f -delete
	find -L ../../LibMultiSpacc -name "*.o" -type f -delete
	rm -f $(AppName)$(ExeSuffix) $(AppName).*$(ExeSuffix)
