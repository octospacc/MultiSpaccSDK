#ifndef _MultiSpacc_Keys_h_
#define _MultiSpacc_Keys_h_

#if defined(MultiSpacc_Target_NES)
	#define MultiSpacc_Key_Action1 PAD_A
#endif

#if defined(MultiSpacc_Target_NES)
	#define MultiSpacc_Key_Action2 PAD_B
#endif

#if defined(MultiSpacc_Target_NES)
	#define MultiSpacc_Key_Confirm PAD_A
#endif

#if defined(MultiSpacc_Target_NES)
	#define MultiSpacc_Key_Cancel PAD_B
#endif

#if defined(MultiSpacc_Target_NES)
	#define MultiSpacc_Key_Start PAD_START
#endif

#if defined(MultiSpacc_Target_SDLCommon) && (defined(MultiSpacc_Target_PC) || defined(MultiSpacc_Target_Web))
	// #define MultiSpacc_Key_Pause SDLK_ESCAPE
	#define MultiSpacc_Key_Pause SDL_SCANCODE_ESCAPE
#elif defined(MultiSpacc_Target_NES)
	#define MultiSpacc_Key_Pause PAD_START
#endif

#if defined(MultiSpacc_Target_SDLCommon) && (defined(MultiSpacc_Target_PC) || defined(MultiSpacc_Target_Web))
	// #define MultiSpacc_Key_Select SDLK_TAB
	#define MultiSpacc_Key_Select SDL_SCANCODE_TAB
#elif defined(MultiSpacc_Target_NES)
	#define MultiSpacc_Key_Select PAD_SELECT
#endif

#if defined(MultiSpacc_Target_SDLCommon) && (defined(MultiSpacc_Target_PC) || defined(MultiSpacc_Target_Web))
	// #define MultiSpacc_Key_Up    SDLK_UP
	// #define MultiSpacc_Key_Down  SDLK_DOWN
	// #define MultiSpacc_Key_Left  SDLK_LEFT
	// #define MultiSpacc_Key_Right SDLK_RIGHT
	#define MultiSpacc_Key_Up    SDL_SCANCODE_UP
	#define MultiSpacc_Key_Down  SDL_SCANCODE_DOWN
	#define MultiSpacc_Key_Left  SDL_SCANCODE_LEFT
	#define MultiSpacc_Key_Right SDL_SCANCODE_RIGHT
#elif defined(MultiSpacc_Target_NDS)
	// ...
#elif defined(MultiSpacc_Target_NES)
	#define MultiSpacc_Key_Up    PAD_UP
	#define MultiSpacc_Key_Down  PAD_DOWN
	#define MultiSpacc_Key_Left  PAD_LEFT
	#define MultiSpacc_Key_Right PAD_RIGHT
#endif

#if defined(MultiSpacc_Target_SDLCommon) && (defined(MultiSpacc_Target_PC) || defined(MultiSpacc_Target_Web))
	// #define MultiSpacc_Key_Action1 SPACE
	// #define MultiSpacc_Key_Action2 SHIFT
	// #define MultiSpacc_Key_Confirm ENTER
	// #define MultiSpacc_Key_Cancel  ESC
#endif

bool MultiSpacc_CheckKey( int key, char pad );

#endif // _MultiSpacc_Keys_h_