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

	// Store key + keyevent.
	std::deque<std::pair<u8, EventSystem::Event<void>>> onKeyTriggered;

	// Loop through all keys and check if user triggered any.
	// If so, invoke key event.
	void HandleKeyTriggered() {
		for (auto& k : onKeyTriggered) {
			if (AEInputCheckTriggered(k.first)) {
				k.second.Invoke();
				return;
			}
		}
	}

	// Subscribe func to key event.
	void SubscribeToKeyTriggered(u8 key, void (*func)()) {
		for (auto& k : onKeyTriggered) {
			// Subscribe to key event if key is found.
			if (k.first == key) {
				k.second.Subscribe(func);
				return;
			}
		}

		// Create and add new key event if key is not found.
		std::pair < u8, EventSystem::Event<void>> keyEvent = { key,{} };
		keyEvent.second.Subscribe(func);
		onKeyTriggered.push_back(keyEvent);
	}

	// Unsubscribe func from key event.
	void UnSubscribeToKeyTriggered(u8 key, void (*func)()) {
		for (auto& k : onKeyTriggered) {
			if (k.first == key) {
				k.second.Unsubscribe(func);
				return;
			}
		}
		std::cout << "KEY NOT FOUND IN OnKeyTriggered DEQUE." << std::endl;
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
