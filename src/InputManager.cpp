
#include <AEEngine.h>
#include <InputManager.h>

namespace InputManager {
	Vec2<int> mousePos;

	EventSystem::Event<Vec2<int>> onMouseClick;
	EventSystem::Event<void> onEscPressed;

	void HandleInput() {
		AEInputGetCursorPosition(&mousePos.x, &mousePos.y);

		// Check mouse-click
		if (AEInputCheckTriggered(VK_LBUTTON)) {
			onMouseClick.Invoke(mousePos);
		}

		// Check escape pressed
		if (AEInputCheckTriggered(VK_ESCAPE)) {
			onEscPressed.Invoke();
		}
	}
}
