/*!************************************************************************
\file InputManager.cpp
\author Tan Jun Rong
\par DP email: t.junrong@digipen.edu
\par Course: CSD1171B
\par Software Engineering Project
\date 15-02-2023
\brief
This source file declares

**************************************************************************/

#include <InputManager.h>
#include <deque>
#include <CardManager.h>
#include <Building.h>

namespace InputManager {
	struct KeyEvent {
		u32 clickTime{};	// Frame time this key was triggered.
		EventSystem::Event<void> onKeyTriggered;
		EventSystem::Event<void> onKeyReleased;
		EventSystem::Event<void> onKeyPressed;
		EventSystem::Event<void> onKeyDoubleClick;
	};

	/*!***********************************************************************
	* FORWARD DECLARATIONS
	*************************************************************************/
	void TryToDrag();
	void StopDragging();

	/*!***********************************************************************
	* CONST VARIABLES
	*************************************************************************/
	const int DOUBLE_CLICK_TIME = 20;	// Amount of frames between clicks triggered to be counted as double clicking.

	/*!***********************************************************************
	* MOUSE RELATED VARIABLES
	*************************************************************************/
	Vec2<int> mousePos{};
	bool isDragging = false;

	std::deque<std::pair<u8, KeyEvent>> keys;

	// Loop through all keys and invoke any events if needed.
	void HandleKeyEvents() {
		for (auto& k : keys) {
			// Key triggered.
			if (AEInputCheckTriggered(k.first)) {
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

	// Subscribe func to key event.
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
					std::cout << "KEY_EVENT_TYPE invalid!" << std::endl;
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
			std::cout << "KEY_EVENT_TYPE invalid!" << std::endl;
			return;
		}
		// Add new key to deque.
		keys.push_back(newKey);
	}

	// Unsubscribe func from key event.
	void UnsubscribeKey(u8 key, KEY_EVENT_TYPE type, void (*func)()) {
		for (auto& k : keys) {
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
				std::cout << "KEY_EVENT_TYPE invalid!" << std::endl;
				return;
			}
		}
		std::cout << "KEY NOT FOUND IN DEQUE." << std::endl;
	}

	void InputManager::Intialize() {
		SubscribeToKey(AEVK_LBUTTON, PRESSED, TryToDrag);
		SubscribeToKey(AEVK_LBUTTON, RELEASED, StopDragging);
	}

	void HandleInput() {
		AEInputGetCursorPosition(&mousePos.x, &mousePos.y);

		// Check and invoke any key events.
		HandleKeyEvents();
	}

	void Free() {
		// Free all key events.
		for (auto& k : keys) {
			k.second.onKeyTriggered.UnsubscribeAll();
			k.second.onKeyPressed.UnsubscribeAll();
			k.second.onKeyReleased.UnsubscribeAll();
			k.second.onKeyDoubleClick.UnsubscribeAll();
		}
	}

	Vec2<int> GetMousePos() {
		return mousePos;
	}

	bool IsDragging() {
		return isDragging;
	}

	void TryToDrag() {
		if (DragDetect(AESysGetWindowHandle(), POINT{ mousePos.x, mousePos.y })) {
			isDragging = true;
		};
	}

	void StopDragging() {
		if (isDragging) isDragging = false;
	}

}
