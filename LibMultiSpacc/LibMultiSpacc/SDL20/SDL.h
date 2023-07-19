#pragma once

#ifndef MultiSpacc_Target_SDL20
	#define MultiSpacc_Target_SDL20
#endif

#include "../MultiSpacc.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_ttf.h"

#define MultiSpacc_Window SDL_Window
#define MultiSpacc_UpdateWindowSurface SDL_UpdateWindowSurface

typedef struct MultiSpacc_Event {
    Uint32 Type;
    SDL_Keycode Key;
} MultiSpacc_Event;
