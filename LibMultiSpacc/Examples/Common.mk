AppName = $(notdir ${CURDIR})
Sources = $(wildcard *.c ../../LibMultiSpacc/*.c)
CFlags = -O2
CC = gcc $(Defines)

ifndef Target
	Target = PC
endif

ifdef Target
	ifeq ($(Target), PC)
		ExeSuffix = .run
		Defines = -DTarget_PC
		MultiSpacc_Target = SDL20
	endif
endif

ifeq ($(MultiSpacc_Target), SDL12)
	Defines += -DMultiSpacc_Target_SDL12
	LdFlags += -lSDL -lSDL_image -lSDL_mixer -lSDL_ttf
	Sources += $(wildcard ../../LibMultiSpacc/SDLCom/*.c ../../LibMultiSpacc/SDL12/*.c)
else ifeq ($(MultiSpacc_Target), SDL20)
	Defines += -DMultiSpacc_Target_SDL20
	LdFlags += -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf
	Sources += $(wildcard ../../LibMultiSpacc/SDLCom/*.c ../../LibMultiSpacc/SDL20/*.c)
endif

Objects = $(Sources:.c=.o)

All: $(AppName)

$(AppName): $(Objects)
	$(CC) $^ $(CFlags) $(LdFlags) -o $(AppName)$(ExeSuffix)

Run: All
	./$(AppName)$(ExeSuffix)

Clean:
	find -L . -name "*.o" -type f -delete
	rm -f $(AppName)$(ExeSuffix) $(AppName).*$(ExeSuffix)

all: All
run: Run
clean: Clean
