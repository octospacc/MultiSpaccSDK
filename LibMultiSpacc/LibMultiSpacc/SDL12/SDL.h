#pragma once

#ifndef MultiSpacc_Target_SDL12
	#define MultiSpacc_Target_SDL12
#endif

#include "../MultiSpacc.h"
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_mixer.h"
#include "SDL/SDL_ttf.h"

#define MultiSpacc_Window SDL_Surface
#define MultiSpacc_UpdateWindowSurface SDL_Flip

typedef struct MultiSpacc_Event {
    Uint32 Type;
    SDLKey Key;
} MultiSpacc_Event;
