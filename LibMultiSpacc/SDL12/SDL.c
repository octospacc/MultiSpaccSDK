#include "../MultiSpacc.h"
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_mixer.h"
#include "SDL/SDL_ttf.h"

SDL_Surface *MultiSpacc_GetWindow( int Width, int Height, int Bits, Uint32 Flags ) {
	return SDL_SetVideoMode( Width, Height, Bits, Flags );
}
SDL_Surface *MultiSpacc_GetWindowSurface( SDL_Surface *Window ) {
	return Window;
}

void MultiSpacc_SetAppTitle( SDL_Surface *Window, const char *Title ) {
	SDL_WM_SetCaption( Title, NULL );
}
void MultiSpacc_SetAppIcon( SDL_Surface *Window, SDL_Surface *Icon ) {
	SDL_WM_SetIcon( Icon, NULL );
}

int MultiSpacc_SetColorKey( SDL_Surface *Surface, bool Flag, Uint32 Key ) {
	if ( Flag ) {
		return SDL_SetColorKey( Surface, SDL_SRCCOLORKEY, Key );
	}
	else {
		return SDL_SetColorKey( Surface, 0, Key );
	}
}
