
#include <AEEngine.h>
#include <InputManager.h>

namespace InputManager {
	Vec2<int> mousePos;

	EventSystem::Event<Vec2<int>> onMouseClick;
	EventSystem::Event<void> onEscPressed;

	EventSystem::Event<void> onButtonPressed;

	void HandleInput() {
		AEInputGetCursorPosition(&mousePos.x, &mousePos.y);

		// Check mouse-click
		if (AEInputCheckTriggered(AEVK_LBUTTON)) {
			onMouseClick.Invoke(mousePos);
		}

		// Check mouse click on button
		if (AEInputCheckTriggered(AEVK_ESCAPE)) {
			onButtonPressed.Invoke();
		}

		// Check escape pressed
		if (AEInputCheckTriggered(AEVK_ESCAPE)) {
			onEscPressed.Invoke();
		}

	}
}
