/*!************************************************************************
\file:          InputManager.h
\author:
\par DP email:
\par Course:    CSD1171B
\par Software Engineering Project
\date:          30-01-2023
\brief


The functions include:
-
**************************************************************************/

#pragma once

#include <MomoMaths.h>
#include <EventSystem.h>
#include <AEEngine.h>

namespace InputManager {
	void HandleInput();
	Vec2<int> GetMousePos();
	void SubscribeToKeyTriggered(u8 key, void (*func)());
	void UnSubscribeToKeyTriggered(u8 key, void (*func)());
}
