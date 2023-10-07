#include "../MultiSpacc.h"
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_mixer.h"
#include "SDL/SDL_ttf.h"
#include "../SDLCom/SDL.h"

MultiSpacc_Window *MultiSpacc_SetWindow( MultiSpacc_SurfaceConfig WindowConfig ) {
	return SDL_SetVideoMode( WindowConfig.Width, WindowConfig.Height, WindowConfig.Bits, WindowConfig.Flags );
}
MultiSpacc_Surface *MultiSpacc_GetWindowSurface( MultiSpacc_Window *Window ) {
	return Window;
}

void MultiSpacc_SetAppTitle( MultiSpacc_Window *Window, const char *Title ) {
	SDL_WM_SetCaption( Title, NULL );
}
void MultiSpacc_SetAppIcon( MultiSpacc_Window *Window, MultiSpacc_Surface *Icon ) {
	SDL_WM_SetIcon( Icon, NULL );
}

int MultiSpacc_SetColorKey( MultiSpacc_Surface *Surface, bool Flag, Uint32 Key ) {
	if( Flag )
		return SDL_SetColorKey( Surface, SDL_SRCCOLORKEY, Key );
	else
		return SDL_SetColorKey( Surface, 0, Key );
}
