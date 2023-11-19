#include "./MultiSpacc.h"

// TODO: Handle actual presses in SDL
// void MultiSpacc_PollButtons( char pad, MultiSpacc_KeysStates *buttonsPressed, MultiSpacc_KeysStates *buttonsHeld )
// {
	// #if defined(MultiSpacc_Target_SDLCommon)
		// SDL_PumpEvents();
		// #if defined(MultiSpacc_Target_SDL12)
			// buttonsPressed->keysStates = SDL_GetKeyState(NULL);
		// #elif defined(MultiSpacc_Target_SDL20)
			// buttonsPressed->keysStates = SDL_GetKeyboardState(NULL);
		// #endif
		// buttonsHeld->keysStates = buttonsPressed->keysStates;

	// #elif defined(MultiSpacc_Target_NDS)
		// scanKeys();
		// buttonsPressed->keysStates = keysDown();
		// buttonsHeld->keysStates = buttonsPressed->keysStates;

	// #elif defined(MultiSpacc_Target_NES)
		// buttonsPressed->keysStates = pad_trigger(pad);
		// buttonsHeld->keysStates = pad_state(pad);

	// #endif
// }

// bool MultiSpacc_CheckButtonState( int button, MultiSpacc_KeysStates *buttonsStates )
// {
	// #if defined(MultiSpacc_Target_SDLCommon)
		// return buttonsStates->keysStates[button];
	// #elif defined(MultiSpacc_Target_NDS) || defined(MultiSpacc_Target_NES)
		// return ( buttonsStates->keysStates & button );
	// #endif
// }

void MultiSpacc_PollButtons( char pad, MultiSpacc_KeysStates *keysStates )
{
	#if defined(MultiSpacc_Target_SDLCommon)
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

	#elif defined(MultiSpacc_Target_NDS)
		scanKeys();
		keysStates->keysPressed = keysDown();
		keysStates->keysHeld = keysStates->keysPressed;

	#elif defined(MultiSpacc_Target_NES)
		keysStates->keysPressed = pad_trigger(pad);
		keysStates->keysHeld = pad_state(pad);

	#endif
}

bool MultiSpacc_CheckKeyPress( int key, MultiSpacc_KeysStates *keysStates )
{
	#if defined(MultiSpacc_Target_SDLCommon)
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
	#elif defined(MultiSpacc_Target_NDS) || defined(MultiSpacc_Target_NES)
		return ( keysStates->keysPressed & key );
	#endif
}

bool MultiSpacc_CheckKeyHold( int key, MultiSpacc_KeysStates *keysStates )
{
	#if defined(MultiSpacc_Target_SDLCommon)
		return keysStates->keysHeld[key];
	#elif defined(MultiSpacc_Target_NDS) || defined(MultiSpacc_Target_NES)
		return ( keysStates->keysHeld & key );
	#endif
}
