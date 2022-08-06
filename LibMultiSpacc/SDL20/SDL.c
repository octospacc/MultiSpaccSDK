#include "../MultiSpacc.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_ttf.h"

SDL_Window *MultiSpacc_GetWindow( int Width, int Height, int Bits, Uint32 Flags ) {
	return SDL_CreateWindow(NULL, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, Width, Height, Flags);
}
SDL_Surface *MultiSpacc_GetWindowSurface( SDL_Surface *Window ) {
	return SDL_GetWindowSurface( Window );
}

void MultiSpacc_SetAppTitle( SDL_Surface *Window, const char *Title ) {
	SDL_SetWindowTitle( Window, Title );
}
void MultiSpacc_SetAppIcon( SDL_Surface *Window, SDL_Surface *Icon ) {
	SDL_SetWindowIcon( Window, Icon );
}

int MultiSpacc_SetColorKey( SDL_Surface *Surface, bool Flag, Uint32 Key ) {
	if ( Flag ) {
		return SDL_SetColorKey( Surface, SDL_TRUE, Key );
	}
	else {
		return SDL_SetColorKey( Surface, SDL_FALSE, Key );
	}
}
