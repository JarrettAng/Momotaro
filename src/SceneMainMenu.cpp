/*!************************************************************************
\file:          SceneMainMenu.cpp
\author:
\par DP email:
\par Course:    CSD1171B
\par Software Engineering Project
\date:          30-01-2023
\brief


The functions include:
-
**************************************************************************/

#include <AEEngine.h>
#include <InputManager.h>
#include <RenderSystem.h>

#include <SceneManager.h>

#include <TextureManager.h>
#include <SceneMainMenu.h>

/*!***********************************************************************
* FORWARD DECLARATIONS
*************************************************************************/
void loadStart();
void loadEditor();
void loadOptions();
void loadControls();
void loadCredits();
void loadQuit();

void handleBtnClick();

void InitializeButtons();

RenderSystem::Interactable startBtn{};
RenderSystem::Interactable editorBtn{};
RenderSystem::Interactable optionBtn{};
RenderSystem::Interactable controlsBtn{};
RenderSystem::Interactable creditsBtn{};
RenderSystem::Interactable quitBtn{};

std::vector<RenderSystem::Interactable> buttons;

void SceneMainMenu::Load() {
}

void SceneMainMenu::Initialize() {
	InitializeButtons();
	InputManager::SubscribeToKey(AEVK_LBUTTON, InputManager::TRIGGERED, handleBtnClick);
}

void SceneMainMenu::Update() {
	// Check for button presses.
	InputManager::HandleInput();
}

void SceneMainMenu::Draw() {
	// Draw menu
	RenderSystem::AddRectToBatch(RenderSystem::UI_BATCH, AEGfxGetWinMinX(), -AEGfxGetWinMinY(), AEGetWindowWidth(), AEGetWindowHeight(), TextureManager::MENU_BG, -1);

	// Draw buttons.
	for (RenderSystem::Interactable& btn : buttons) {
		RenderSystem::AddRectToBatch(RenderSystem::UI_BATCH, btn.render.rect.transform.pos.x, btn.render.rect.transform.pos.y, btn.render.rect.transform.size.x, btn.render.rect.transform.size.y, btn.render.rect.graphics.tex);
	}

	RenderSystem::Render();
}

void SceneMainMenu::Free() {
	InputManager::Free();
}

void SceneMainMenu::Unload() {
}

void handleBtnClick() {
	Vec2<int> mousePos = InputManager::GetMousePos();

	std::cout << "mousePos.x: " << mousePos.x - AEGetWindowWidth() / 2 << " mousePos.y: " << mousePos.y - AEGetWindowHeight() / 2 << std::endl << std::endl;
	std::cout << "minX: " << buttons[0].render.rect.transform.pos.x << " minY: " << buttons[0].render.rect.transform.pos.y << std::endl;
	std::cout << "maxX: " << buttons[0].render.rect.transform.pos.x + buttons[0].render.rect.transform.size.x << " maxY: " << buttons[0].render.rect.transform.pos.y + buttons[0].render.rect.transform.size.y << std::endl << std::endl;

	for (RenderSystem::Interactable& btn : buttons) {
		// LEFT / RIGHT BOUNDS
		if ((mousePos.x - AEGetWindowWidth() / 2 > btn.render.rect.transform.pos.x &&
			mousePos.x - AEGetWindowWidth() / 2 < btn.render.rect.transform.pos.x + btn.render.rect.transform.size.x) &&

			// TOP / BOTTOM BOUNDS
			(mousePos.y - AEGetWindowHeight() / 2 > -btn.render.rect.transform.pos.y &&
				mousePos.y - AEGetWindowHeight() / 2 < -btn.render.rect.transform.pos.y + btn.render.rect.transform.size.y)) {
			btn.func();
			break;
		}
	}
}

void InitializeButtons() {
	// Position button based on window width / height.
	// Width and height of button is based on png size.

	startBtn.render.rect.graphics.tex = TextureManager::STARTGAME_BTN;
	startBtn.func = loadStart;

	startBtn.render.rect.transform.pos.x = AEGfxGetWinMinX() * 0.525;
	startBtn.render.rect.transform.pos.y = AEGfxGetWinMinY() * 0.222;

	startBtn.render.rect.transform.size.x = 460;
	startBtn.render.rect.transform.size.y = 100;

	buttons.push_back(startBtn);

	editorBtn.render.rect.graphics.tex = TextureManager::EDITOR_BTN;
	editorBtn.func = loadEditor;

	editorBtn.render.rect.transform.pos.x = AEGfxGetWinMaxX() * 0.237;
	editorBtn.render.rect.transform.pos.y = AEGfxGetWinMinY() * 0.222;

	editorBtn.render.rect.transform.size.x = 260;
	editorBtn.render.rect.transform.size.y = 100;
	buttons.push_back(editorBtn);

	optionBtn.render.rect.graphics.tex = TextureManager::OPTIONS_BTN;
	optionBtn.func = loadOptions;

	optionBtn.render.rect.transform.pos.x = AEGfxGetWinMinX() * 0.525;
	optionBtn.render.rect.transform.pos.y = AEGfxGetWinMinY() * 0.488;

	optionBtn.render.rect.transform.size.x = 300;
	optionBtn.render.rect.transform.size.y = 100;
	buttons.push_back(optionBtn);

	controlsBtn.render.rect.graphics.tex = TextureManager::CONTROLS_BTN;
	controlsBtn.func = loadControls;

	controlsBtn.render.rect.transform.pos.x = AEGfxGetWinMaxX() * 0.112;
	controlsBtn.render.rect.transform.pos.y = AEGfxGetWinMinY() * 0.488;

	controlsBtn.render.rect.transform.size.x = 360;
	controlsBtn.render.rect.transform.size.y = 100;
	buttons.push_back(controlsBtn);

	creditsBtn.render.rect.graphics.tex = TextureManager::CREDITS_BTN;
	creditsBtn.func = loadCredits;

	creditsBtn.render.rect.transform.pos.x = AEGfxGetWinMinX() * 0.975;
	creditsBtn.render.rect.transform.pos.y = AEGfxGetWinMinY() * 0.733;

	creditsBtn.render.rect.transform.size.x = 300;
	creditsBtn.render.rect.transform.size.y = 100;
	buttons.push_back(creditsBtn);

	quitBtn.render.rect.graphics.tex = TextureManager::QUIT_BTN;
	quitBtn.func = loadQuit;

	quitBtn.render.rect.transform.pos.x = AEGfxGetWinMaxX() * 0.762;
	quitBtn.render.rect.transform.pos.y = AEGfxGetWinMinY() * 0.733;

	quitBtn.render.rect.transform.size.x = 170;
	quitBtn.render.rect.transform.size.y = 110;
	buttons.push_back(quitBtn);
}

void loadStart() {
	SceneManager::LoadScene(SceneManager::GAME_LEVEL);
}

void loadEditor() {
	SceneManager::LoadScene(SceneManager::EDITOR);
}

void loadOptions() {
	SceneManager::LoadScene(SceneManager::OPTIONS);
}

void loadControls() {
	SceneManager::LoadScene(SceneManager::CONTROLS);
}

void loadCredits() {
	SceneManager::LoadScene(SceneManager::CREDITS);
}

void loadQuit() {
	SceneManager::LoadScene(SceneManager::QUIT);
}

