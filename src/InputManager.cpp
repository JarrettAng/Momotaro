
#include <AEEngine.h>
#include <InputManager.h>

namespace InputManager {
	Vec2<int> mousePos;

	EventSystem::Event<Vec2<int>> onMouseClick;
	EventSystem::Event<Vec2<int>> onMouseRightClick;
	EventSystem::Event<void> onEscPressed;
	
	EventSystem::Event<void> onCKeyPressed;
	EventSystem::Event<Vec2<int>> onNKeyPressed;
	EventSystem::Event<void> onRKeyPressed;
	EventSystem::Event<Vec2<int>> on1KeyPressed;
	EventSystem::Event<Vec2<int>> on2KeyPressed;
	EventSystem::Event<Vec2<int>> on3KeyPressed;

	EventSystem::Event<void> onButtonPressed;

	void HandleInput() {
		AEInputGetCursorPosition(&mousePos.x, &mousePos.y);

		// Check mouse-click
		if (AEInputCheckTriggered(AEVK_LBUTTON)) {
			onMouseClick.Invoke(mousePos);
		}
		//Check mouse-right click
		if (AEInputCheckTriggered(AEVK_RBUTTON)) {
			onMouseRightClick.Invoke(mousePos);
		}
		//C
		if (AEInputCheckTriggered(AEVK_C)) {
			onCKeyPressed.Invoke();
		}
		//R
		if (AEInputCheckTriggered(AEVK_R)) {
			onRKeyPressed.Invoke();
		}
		//1
		if (AEInputCheckTriggered(AEVK_1)) {
			on1KeyPressed.Invoke(mousePos);
		}
		//2
		if (AEInputCheckTriggered(AEVK_2)) {
			on2KeyPressed.Invoke(mousePos);
		}
		//3
		if (AEInputCheckTriggered(AEVK_3)) {
			on3KeyPressed.Invoke(mousePos);
		}
		//N
		if (AEInputCheckTriggered(AEVK_N)) {
			onNKeyPressed.Invoke(mousePos);
		}



		// Check escape pressed
		if (AEInputCheckTriggered(AEVK_ESCAPE)) {
			onEscPressed.Invoke();
			// Check mouse click on button
			onButtonPressed.Invoke();
		}

	}
}
