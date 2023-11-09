#include "./MultiSpacc.h"

bool MultiSpacc_CheckKey( int key, char pad )
{
	#if defined(MultiSpacc_Target_SDLCom)
		SDL_Event event;
		while( SDL_PollEvent(&event) ) // TODO: fix this, it eliminates all events beside the first
		{
			if ( event.type == SDL_KEYDOWN && event.key.keysym.sym == key )
			{
				return true;
			}
		}
		return false;
	#elif defined(MultiSpacc_Target_NES)
		return ( pad_poll(pad) & key );
	#endif
}
