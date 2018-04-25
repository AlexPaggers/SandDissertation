#ifndef _GAME_DATA_H_
#define _GAME_DATA_H_

#include <dinput.h>

struct GameData
{
	float m_dt;  //delta time
	float m_FPS = 0;

	unsigned char* m_keyboardState;
	unsigned char* m_prevKeyboardState;
	DIMOUSESTATE* m_mouseState;

	float m_zWall = 10;
	float m_yWall = 10;
	float m_xWall = 10;

};
#endif