#ifndef _GAME_DATA_H_
#define _GAME_DATA_H_

#include <dinput.h>

struct GameData
{
	float m_dt;  //delta time

	unsigned char* m_keyboardState;
	unsigned char* m_prevKeyboardState;
	DIMOUSESTATE* m_mouseState;
};
#endif