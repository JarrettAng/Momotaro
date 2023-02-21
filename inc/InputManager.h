/*!************************************************************************
\file InputManager.h
\author Tan Jun Rong
\par DP email: t.junrong@digipen.edu
\par Course: CSD1171B
\par Software Engineering Project
\date 15-02-2023
\brief
This header file declares

**************************************************************************/

#pragma once

#include <MomoMaths.h>
#include <EventSystem.h>
#include <AEEngine.h>
#include <deque>

namespace InputManager {

	enum KEY_EVENT_TYPE {
		TRIGGERED,
		RELEASED,
		PRESSED,
		D_CLICK
	};
	void Intialize();
	void Free();
	void HandleInput();
	Vec2<int> GetMousePos();
	bool IsDragging();
	void SubscribeToKey(u8 key, KEY_EVENT_TYPE type, void (*func)());
	void UnsubscribeKey(u8 key, KEY_EVENT_TYPE type, void (*func)());
}
