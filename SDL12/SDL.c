#include <stddef.h>

void MultiSpacc_SetAppTitle( const char *Title ) {
	SDL_WM_SetCaption( Title, NULL );
}
