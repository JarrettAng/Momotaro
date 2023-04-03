/*!************************************************************************
\file InputManager.h
\author Tan Jun Rong
\par DP email: t.junrong@digipen.edu
\par Course: CSD1171B
\par Software Engineering Project
\date 15-02-2023
\brief
This header file contains prototype functions and declarations for InputManager.cpp.

All content © 2023 DigiPen Institute of Technology Singapore.
All rights reserved.
**************************************************************************/

#pragma once
///////////////////////////////////////////////////////////////////////////
#include <MomoMaths.h>
#include <EventSystem.h>
#include <AEEngine.h>
#include <deque>
///////////////////////////////////////////////////////////////////////////

namespace InputManager {

	/*!***********************************************************************
	\brief
		Contains type of key events for each key.
	*************************************************************************/
	enum KEY_EVENT_TYPE {
		TRIGGERED,
		RELEASED,
		PRESSED,
		D_CLICK
	};

	/*!***********************************************************************
	\brief
		Initialize InputManager.
	*************************************************************************/
	void Initialize();

	/*!***********************************************************************
	\brief
		Free InputManager.
	*************************************************************************/
	void Free();

	/*!***********************************************************************
	\brief
		Update InputManager.
	*************************************************************************/
	void HandleInput();

	/*!***********************************************************************
	\brief
		Get mouse position.
	*************************************************************************/
	Vec2<int> GetMousePos();

	/*!***********************************************************************
	\brief
		Get mouse delta position.
	*************************************************************************/
	Vec2<int> GetMousePosDelta();

	/*!***********************************************************************
	\brief
		Get whether if player is dragging.
	*************************************************************************/
	bool IsDragging();

	/*!***********************************************************************
	\brief
		Check if the mouse has moved.
	*************************************************************************/
	bool HasMouseMoved();

	/*!***********************************************************************
	\brief
		Subscribe func to key event.
	*************************************************************************/
	void SubscribeToKey(u8 key, KEY_EVENT_TYPE type, void (*func)());

	/*!***********************************************************************
	\brief
		Unsubscribe func from key event.
	*************************************************************************/
	void UnsubscribeKey(u8 key, KEY_EVENT_TYPE type, void (*func)());
}
