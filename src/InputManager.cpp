
#include <AEEngine.h>
#include <InputManager.h>

namespace InputManager {
	Vec2<int> mousePos;

	void HandleInput() {
		AEInputGetCursorPosition(&mousePos.x, &mousePos.y);
	}
}
