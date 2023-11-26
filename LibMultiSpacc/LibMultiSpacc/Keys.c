#include "./MultiSpacc.h"

void MultiSpacc_PollButtons( char pad, MultiSpacc_KeysStates *keysStates )
{
	#if defined(MultiSpacc_Target_SDLCommon)
		#if defined(MultiSpacc_Target_Switch) // WIP
			keysStates->keysHeld = SDL_JoystickOpen(pad);

		#else
			int numkeys;

			SDL_PumpEvents();

			#if defined(MultiSpacc_Target_SDL12)
				keysStates->keysHeld = SDL_GetKeyState(&numkeys);
			#elif defined(MultiSpacc_Target_SDL20)
				keysStates->keysHeld = SDL_GetKeyboardState(&numkeys);
			#endif

			if( keysStates->keysPressed == NULL )
			{
				keysStates->keysPressed = malloc( numkeys*sizeof(int) );
				memset( (void*)keysStates->keysPressed, false, numkeys*sizeof(int) );
			}

		#endif

	#elif defined(MultiSpacc_Target_NDS)
		scanKeys();
		keysStates->keysPressed = keysDown(); //keysHeld();
		keysStates->keysHeld = keysStates->keysPressed;

	#elif defined(MultiSpacc_Target_NES)
		keysStates->keysPressed = pad_trigger(pad);
		keysStates->keysHeld = pad_state(pad);

	#endif
}

bool MultiSpacc_CheckKeyPress( int key, MultiSpacc_KeysStates *keysStates )
{
	#if defined(MultiSpacc_Target_SDLCommon)
		#if defined(MultiSpacc_Target_Switch) // WIP
			return SDL_JoystickGetButton( keysStates->keysHeld, key );
		#else
			if ( keysStates->keysHeld[key] && !keysStates->keysPressed[key] )
			{
				keysStates->keysPressed[key] = true;
				return true;
			}
			else if ( !keysStates->keysHeld[key] && keysStates->keysPressed[key] )
			{
				keysStates->keysPressed[key] = false;
			}
			return false;
		#endif
	#elif defined(MultiSpacc_Target_NDS) || defined(MultiSpacc_Target_NES)
		return ( keysStates->keysPressed & key );
	#endif
}

bool MultiSpacc_CheckKeyHold( int key, MultiSpacc_KeysStates *keysStates )
{
	#if defined(MultiSpacc_Target_SDLCommon)
		#if defined(MultiSpacc_Target_Switch)
			return SDL_JoystickGetButton( keysStates->keysHeld, key );
		#else
			return keysStates->keysHeld[key];
		#endif
	#elif defined(MultiSpacc_Target_NDS) || defined(MultiSpacc_Target_NES)
		return ( keysStates->keysHeld & key );
	#endif
}
