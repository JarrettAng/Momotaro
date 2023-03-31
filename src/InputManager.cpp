/*!************************************************************************
\file InputManager.cpp
\author Tan Jun Rong
\par DP email: t.junrong@digipen.edu
\par Course: CSD1171B
\par Software Engineering Project
\date 15-02-2023
\brief
This source file handles the input of the player.
**************************************************************************/

#include <InputManager.h>
#include <AudioManager.h>
#include <DebugManager.h>

namespace InputManager {
	/*!***********************************************************************
	\brief
		Contains the events of a key.
	*************************************************************************/
	struct KeyEvent {
		u32 clickTime{};	// Frame time this key was triggered.
		EventSystem::Event<void> onKeyTriggered;
		EventSystem::Event<void> onKeyReleased;
		EventSystem::Event<void> onKeyPressed;
		EventSystem::Event<void> onKeyDoubleClick;
	};

	/*!***********************************************************************
	* CONST VARIABLES
	*************************************************************************/
	const int DOUBLE_CLICK_TIME = 20;	// Amount of frames between clicks triggered to be counted as double clicking.

	/*!***********************************************************************
	* FORWARD DECLARATIONS
	*************************************************************************/
	void TryToDrag();
	void StopDragging();
	void HandleKeyEvents();

	/*!***********************************************************************
	* MOUSE RELATED VARIABLES
	*************************************************************************/
	Vec2<int> mousePos{};
	Vec2<int> mousePosDelta{};
	bool isDragging = false;

	std::deque<std::pair<u8, KeyEvent>> keys;

	/*!***********************************************************************
	\brief
		Loop through all keys and invoke any events if needed.
	*************************************************************************/
	void HandleKeyEvents() {
		for (auto& k : keys) {
			// Key triggered.
			if (AEInputCheckTriggered(k.first)) {
				// Click sound.
				AudioManager::PlayAudioClip(AudioManager::ClipName::SFX_CLICK);
				k.second.onKeyTriggered.Invoke();

				// Check if player is double clicking based on time between key triggered.
				if (AEFrameRateControllerGetFrameCount() - k.second.clickTime < DOUBLE_CLICK_TIME) {
					k.second.onKeyDoubleClick.Invoke();
				}

				// Cache time when player clicked.
				k.second.clickTime = AEFrameRateControllerGetFrameCount();
				continue;
			}

			// Key released.
			if (AEInputCheckReleased(k.first)) {
				k.second.onKeyReleased.Invoke();
				continue;
			}

			// Key pressed down.
			if (AEInputCheckPrev(k.first) && AEInputCheckCurr(k.first)) {
				k.second.onKeyPressed.Invoke();
				continue;
			}
		}
	}

	/*!***********************************************************************
	\brief
		Subscribe func to key event.
	\param key
		Which key.
	\param type
		Type of key event to call function.
	\param func
		Callback function when key event is invoke.
	*************************************************************************/
	void SubscribeToKey(u8 key, KEY_EVENT_TYPE type, void (*func)()) {
		for (auto& k : keys) {
			// Subscribe to key event if key is found.
			if (k.first == key) {
				switch (type) {
				case InputManager::TRIGGERED:
					k.second.onKeyTriggered.Subscribe(func);
					return;
				case InputManager::RELEASED:
					k.second.onKeyReleased.Subscribe(func);
					return;
				case InputManager::PRESSED:
					k.second.onKeyPressed.Subscribe(func);
					return;
				case InputManager::D_CLICK:
					k.second.onKeyDoubleClick.Subscribe(func);
					return;
				default:
					Debug::PrintError(__FILE__, __LINE__, "KEY_EVENT_TYPE invalid!");
					return;
				}
			}
		}

		// Create new key if key is not found.
		std::pair < u8, KeyEvent> newKey = { key,{} };

		// Subscribe to key event.
		switch (type) {
		case InputManager::TRIGGERED:
			newKey.second.onKeyTriggered.Subscribe(func);
			break;
		case InputManager::RELEASED:
			newKey.second.onKeyReleased.Subscribe(func);
			break;
		case InputManager::PRESSED:
			newKey.second.onKeyPressed.Subscribe(func);
			break;
		case InputManager::D_CLICK:
			newKey.second.onKeyDoubleClick.Subscribe(func);
			break;
		default:
			Debug::PrintError(__FILE__, __LINE__, "KEY_EVENT_TYPE invalid!");
			return;
		}
		// Add new key to deque.
		keys.push_back(newKey);
	}

	/*!***********************************************************************
	\brief
		Unsubscribe func from key event.
	\param key
		Which key.
	\param type
		Type of key event to unsubscribe function.
	\param func
		Function to unsubscribe.
	*************************************************************************/
	void UnsubscribeKey(u8 key, KEY_EVENT_TYPE type, void (*func)()) {
		for (auto& k : keys) {
			if (k.first != key) continue;

			switch (type) {
			case InputManager::TRIGGERED:
				k.second.onKeyTriggered.Unsubscribe(func);
				return;
			case InputManager::RELEASED:
				k.second.onKeyReleased.Unsubscribe(func);
				return;
			case InputManager::PRESSED:
				k.second.onKeyPressed.Unsubscribe(func);
				return;
			case InputManager::D_CLICK:
				k.second.onKeyDoubleClick.Unsubscribe(func);
				return;
			default:
				Debug::PrintError(__FILE__, __LINE__, "KEY_EVENT_TYPE invalid!");
				return;
			}
		}
		Debug::PrintError(__FILE__, __LINE__, "KEY NOT FOUND IN DEQUE.");
	}

	/*!***********************************************************************
	\brief
		Initialize InputManager.
	*************************************************************************/
	void Initialize() {
		// Initialize dragging functionalities.
		SubscribeToKey(AEVK_LBUTTON, PRESSED, TryToDrag);
		SubscribeToKey(AEVK_LBUTTON, RELEASED, StopDragging);
	}

	/*!***********************************************************************
	\brief
		Update InputManager.
	*************************************************************************/
	void HandleInput() {
		// Update mouse position.
		AEInputGetCursorPosition(&mousePos.x, &mousePos.y);
		AEInputGetCursorPositionDelta(&mousePosDelta.x, &mousePosDelta.y);

		// Check and invoke any key events.
		HandleKeyEvents();
	}

	/*!***********************************************************************
	\brief
		Check if the mouse has moved.
	\return
		True / false.
	*************************************************************************/
	bool HasMouseMoved() {
		return (mousePosDelta.x != 0 && mousePosDelta.y != 0);
	}

	/*!***********************************************************************
	\brief
		Free InputManager.
	*************************************************************************/
	void Free() {
		// Free all key events.
		for (auto& k : keys) {
			k.second.onKeyTriggered.UnsubscribeAll();
			k.second.onKeyPressed.UnsubscribeAll();
			k.second.onKeyReleased.UnsubscribeAll();
			k.second.onKeyDoubleClick.UnsubscribeAll();
		}
	}

	/*!***********************************************************************
	\brief
		Get mouse delta position.
	\return
		Mouse delta position.
	*************************************************************************/
	Vec2<int> GetMousePosDelta() {
		return mousePosDelta;
	}

	/*!***********************************************************************
	\brief
		Get mouse position.
	\return
		Mouse position.
	*************************************************************************/
	Vec2<int> GetMousePos() {
		return mousePos;
	}

	/*!***********************************************************************
	\brief
		Get whether if player is dragging.
	\return
		True / false.
	*************************************************************************/
	bool IsDragging() {
		return isDragging;
	}

	/*!***********************************************************************
	\brief
		Check if player is dragging.
	*************************************************************************/
	void TryToDrag() {
		// Check if player is trying to drag.
		if (!isDragging && DragDetect(AESysGetWindowHandle(), POINT{ mousePos.x, mousePos.y })) {
			isDragging = true;
		};
	}

	/*!***********************************************************************
	\brief
		Stop dragging.
	*************************************************************************/
	void StopDragging() {
		if (isDragging) isDragging = false;
	}
}
