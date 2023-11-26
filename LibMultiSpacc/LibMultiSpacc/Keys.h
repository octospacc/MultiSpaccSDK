#ifndef _MultiSpacc_Keys_h_
#define _MultiSpacc_Keys_h_

#ifdef __cplusplus
extern "C" {
#endif

#if defined(MultiSpacc_Target_SDL12)
	#define MultiSpacc_SDLK_ESCAPE SDLK_ESCAPE
	#define MultiSpacc_SDLK_RETURN SDLK_RETURN
	#define MultiSpacc_SDLK_TAB    SDLK_TAB
	#define MultiSpacc_SDLK_UP     SDLK_UP
	#define MultiSpacc_SDLK_DOWN   SDLK_DOWN
	#define MultiSpacc_SDLK_LEFT   SDLK_LEFT
	#define MultiSpacc_SDLK_RIGHT  SDLK_RIGHT
#elif defined(MultiSpacc_Target_SDL20)
	#define MultiSpacc_SDLK_ESCAPE SDL_SCANCODE_ESCAPE
	#define MultiSpacc_SDLK_RETURN SDL_SCANCODE_RETURN
	#define MultiSpacc_SDLK_TAB    SDL_SCANCODE_TAB
	#define MultiSpacc_SDLK_UP     SDL_SCANCODE_UP
	#define MultiSpacc_SDLK_DOWN   SDL_SCANCODE_DOWN
	#define MultiSpacc_SDLK_LEFT   SDL_SCANCODE_LEFT
	#define MultiSpacc_SDLK_RIGHT  SDL_SCANCODE_RIGHT
#endif

#if defined(MultiSpacc_Target_Switch) && defined(MultiSpacc_Target_SDLCommon)
	#define JOY_A     0
	#define JOY_B     1
	#define JOY_X     2
	#define JOY_Y     3
	#define JOY_PLUS  10
	#define JOY_MINUS 11
	#define JOY_LEFT  12
	#define JOY_UP    13
	#define JOY_RIGHT 14
	#define JOY_DOWN  15
#endif

// Action1
#if defined(MultiSpacc_Target_NES)
	#define MultiSpacc_Key_Action1 PAD_A
#endif

// Action2
#if defined(MultiSpacc_Target_NES)
	#define MultiSpacc_Key_Action2 PAD_B
#endif

// Confirm
#if defined(MultiSpacc_Target_SDLCommon) && (defined(MultiSpacc_Target_PC) || defined(MultiSpacc_Target_Web))
	#define MultiSpacc_Key_Confirm MultiSpacc_SDLK_RETURN
#elif defined(MultiSpacc_Target_NES)
	#define MultiSpacc_Key_Confirm PAD_A
#endif

// Cancel
#if defined(MultiSpacc_Target_NES)
	#define MultiSpacc_Key_Cancel PAD_B
#endif

// Start
#if defined(MultiSpacc_Target_NES)
	#define MultiSpacc_Key_Start PAD_START
#endif

// Pause
#if defined(MultiSpacc_Target_SDLCommon) && (defined(MultiSpacc_Target_PC) || defined(MultiSpacc_Target_Web))
	#define MultiSpacc_Key_Pause MultiSpacc_SDLK_ESCAPE
#elif defined(MultiSpacc_Target_SDLCommon) && defined(MultiSpacc_Target_Switch)
	#define MultiSpacc_Key_Pause JOY_PLUS
#elif defined(MultiSpacc_Target_NDS)
	#define MultiSpacc_Key_Pause KEY_START
#elif defined(MultiSpacc_Target_NES)
	#define MultiSpacc_Key_Pause PAD_START
#endif

// Select
#if defined(MultiSpacc_Target_SDLCommon) && (defined(MultiSpacc_Target_PC) || defined(MultiSpacc_Target_Web))
	#define MultiSpacc_Key_Select MultiSpacc_SDLK_TAB
#elif defined(MultiSpacc_Target_NDS)
	#define MultiSpacc_Key_Select KEY_SELECT
#elif defined(MultiSpacc_Target_NES)
	#define MultiSpacc_Key_Select PAD_SELECT
#endif

// Directions
#if defined(MultiSpacc_Target_SDLCommon) && (defined(MultiSpacc_Target_PC) || defined(MultiSpacc_Target_Web))
	#define MultiSpacc_Key_Up    MultiSpacc_SDLK_UP
	#define MultiSpacc_Key_Down  MultiSpacc_SDLK_DOWN
	#define MultiSpacc_Key_Left  MultiSpacc_SDLK_LEFT
	#define MultiSpacc_Key_Right MultiSpacc_SDLK_RIGHT
#elif defined(MultiSpacc_Target_NDS)
	#define MultiSpacc_Key_Up    KEY_UP
	#define MultiSpacc_Key_Down  KEY_DOWN
	#define MultiSpacc_Key_Left  KEY_LEFT
	#define MultiSpacc_Key_Right KEY_RIGHT
#elif defined(MultiSpacc_Target_NES)
	#define MultiSpacc_Key_Up    PAD_UP
	#define MultiSpacc_Key_Down  PAD_DOWN
	#define MultiSpacc_Key_Left  PAD_LEFT
	#define MultiSpacc_Key_Right PAD_RIGHT
#endif

#if defined(MultiSpacc_Target_SDLCommon) && (defined(MultiSpacc_Target_PC) || defined(MultiSpacc_Target_Web))
	// #define MultiSpacc_Key_Action1 SPACE
	// #define MultiSpacc_Key_Action2 SHIFT
	// #define MultiSpacc_Key_Cancel  ESC
#endif

typedef struct MultiSpacc_KeysStates {
	#if defined(MultiSpacc_Target_SDLCommon)
		Uint8 *keysPressed;
		const Uint8 *keysHeld;
	#elif defined(MultiSpacc_Target_NDS)
		int keysPressed;
		int keysHeld;
	#elif defined(MultiSpacc_Target_NES)
		char keysPressed;
		char keysHeld;
	#endif
} MultiSpacc_KeysStates;

void MultiSpacc_PollButtons( char pad, MultiSpacc_KeysStates *keysStates );
bool MultiSpacc_CheckKeyPress( int key, MultiSpacc_KeysStates *keysStates );
bool MultiSpacc_CheckKeyHold( int key, MultiSpacc_KeysStates *keysStates );

#ifdef __cplusplus
}
#endif

#endif // _MultiSpacc_Keys_h_
