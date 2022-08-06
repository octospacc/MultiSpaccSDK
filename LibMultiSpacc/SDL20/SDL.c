#include "../MultiSpacc.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_ttf.h"

MultiSpacc_Window *MultiSpacc_SetWindow( int Width, int Height, int Bits, Uint32 Flags ) {
	return SDL_CreateWindow(NULL, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, Width, Height, Flags);
}
MultiSpacc_Surface *MultiSpacc_GetWindowSurface( MultiSpacc_Window *Window ) {
	return SDL_GetWindowSurface( Window );
}

void MultiSpacc_SetAppTitle( MultiSpacc_Window *Window, const char *Title ) {
	SDL_SetWindowTitle( Window, Title );
}
void MultiSpacc_SetAppIcon( MultiSpacc_Window *Window, MultiSpacc_Surface *Icon ) {
	SDL_SetWindowIcon( Window, Icon );
}

int MultiSpacc_SetColorKey( MultiSpacc_Surface *Surface, bool Flag, Uint32 Key ) {
	if ( Flag ) {
		return SDL_SetColorKey( Surface, SDL_TRUE, Key );
	}
	else {
		return SDL_SetColorKey( Surface, SDL_FALSE, Key );
	}
}
