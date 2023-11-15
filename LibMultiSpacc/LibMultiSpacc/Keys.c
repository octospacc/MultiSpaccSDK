#include "./MultiSpacc.h"

bool MultiSpacc_CheckKey( int key, char pad )
{
	#if defined(MultiSpacc_Target_SDLCommon)
		#if defined(MultiSpacc_Target_SDL12)
			Uint8 *keys = SDL_GetKeyState(NULL);
		#elif defined(MultiSpacc_Target_SDL20)
			const Uint8 *keys;
			SDL_PumpEvents();
			keys = SDL_GetKeyboardState(NULL);
		#endif
		return keys[key];
		// SDL_Event event;
		// while( SDL_PollEvent(&event) ) // TODO: fix this, it eliminates all events beside the first
		// {
			// if ( event.type == SDL_KEYDOWN && event.key.keysym.sym == key )
			// {
				// return true;
			// }
		// }
		// return false;
	#elif defined(MultiSpacc_Target_NDS)
		int keys;
		scanKeys();
		keys = keysDown();
		return ( keys & key );
	#elif defined(MultiSpacc_Target_NES)
		return ( pad_poll(pad) & key );
	#endif
}
