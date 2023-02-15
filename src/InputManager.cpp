/*!************************************************************************
\file:          InputManager.cpp
\author:
\par DP email:
\par Course:    CSD1171B
\par Software Engineering Project
\date:          30-01-2023
\brief


The functions include:
-
**************************************************************************/

#include <InputManager.h>
#include <deque>
namespace InputManager {
	Vec2<int> mousePos{};

	// Store input keys to check for. (WIP, WILL CHANGE)
	std::initializer_list<u8> keys{ AEVK_LBUTTON, AEVK_RBUTTON, AEVK_C, AEVK_R, AEVK_1, AEVK_2, AEVK_3, AEVK_N, AEVK_ESCAPE };
	// Store key + keyevent.
	std::deque<std::pair<u8, EventSystem::Event<void>>> onKeyTriggered;

	// Initialize deque with key and its respective event.
	void InputManager::Initialize() {
		for (const auto& k : keys) {
			onKeyTriggered.push_back(std::pair < u8, EventSystem::Event<void>>{k, {}});
		}
	}

	// Loop through all keys and check if user triggered any.
	// If so, invoke key event.
	void HandleKeyTriggered() {
		for (auto& k : onKeyTriggered) {
			if (AEInputCheckTriggered(k.first)) {
				k.second.Invoke();
			}
		}
	}

	// Subscribe func to key event.
	void SubscribeToKeyTriggered(u8 key, void (*func)()) {
		for (auto& k : onKeyTriggered) {
			if (k.first == key) {
				k.second.Subscribe(func);
			}
		}
	}

	// Unsubscribe func from key event.
	void UnSubscribeToKeyTriggered(u8 key, void (*func)()) {
		for (auto& k : onKeyTriggered) {
			if (k.first == key) {
				k.second.Unsubscribe(func);
			}
		}
	}

	Vec2<int> GetMousePos() {
		return mousePos;
	}

	void HandleInput() {
		AEInputGetCursorPosition(&mousePos.x, &mousePos.y);

		// Check and invoke any key triggered events.
		HandleKeyTriggered();

		// Check for drag.
		if (DragDetect(AESysGetWindowHandle(), POINT{ mousePos.x, mousePos.y })) {
			//std::cout << "IS DRAGGING" << std::endl;
		};
	}
}
