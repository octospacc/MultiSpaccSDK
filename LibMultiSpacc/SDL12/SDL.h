#pragma once

#ifndef MultiSpacc_Target_SDL12
	#define MultiSpacc_Target_SDL12
#endif

#include "../MultiSpacc.h"
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_mixer.h"
#include "SDL/SDL_ttf.h"

SDL_Surface *MultiSpacc_GetWindow( int Width, int Height, int Bits, Uint32 Flags );
SDL_Surface *MultiSpacc_GetWindowSurface( SDL_Surface *Window );

void MultiSpacc_SetAppTitle( SDL_Surface *Window, const char *Title );
void MultiSpacc_SetAppIcon( SDL_Surface *Window, SDL_Surface *Icon );

int MultiSpacc_SetColorKey( SDL_Surface *Surface, bool Flag, Uint32 Key );
