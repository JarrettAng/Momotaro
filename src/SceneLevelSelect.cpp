/*!************************************************************************
\file:          SceneLevelSelect.cpp
\author:
\par DP email:
\par Course:    CSD1171B
\par Software Engineering Project
\date:          29-03-2023
\brief


The functions include:
-
**************************************************************************/

///////////////////////////////////////////////////////////////////////////
#include <AEEngine.h>
#include <RenderSystem.h>

#include <AudioManager.h>
#include <SceneManager.h>
#include <InputManager.h>
#include <GridManager.h>
#include <FileIOManager.h>

#include <Building.h>
#include <SceneLevelSelect.h>
#include <SceneGameLevel.h>
#include <TextureManager.h>
#include <ColorTable.h>
///////////////////////////////////////////////////////////////////////////
// Forward Declaration
void LvlSelectInitializeUI();
void DrawLvlSelectButtons();
void HandleLvlSelectBtnHover();
void HandleLvlSelectBtnClick();

void LvlSelectReturnToMenu(); // Button functions
void LvlSelectLoadMap1();
void LvlSelectLoadMap2();
void LvlSelectLoadMap3();
void LvlSelectLoadUserMap1();
void LvlSelectLoadUserMap2();

///////////////////////////////////////////////////////////////////////////
// UI variables
std::vector<RenderSystem::Interactable> lvlSelectButtons;
RenderSystem::Interactable lvlSelectClickedBtn{};	// Button player clicked on. To get position and callback func.

RenderSystem::Interactable lvlSelectBackBtn{};
RenderSystem::Interactable lastMapBtn{};
RenderSystem::Interactable lvl1Btn{};
RenderSystem::Interactable lvl2Btn{};
RenderSystem::Interactable lvl3Btn{};
RenderSystem::Interactable lvlUser1Btn{};
RenderSystem::Interactable lvlUser2Btn{};

std::vector<LevelPreview> lvlPreviews;

///////////////////////////////////////////////////////////////////////////
// Const variables
const float LVL_SELECT_TRANSITION_TIME = 1.0f;
const float LVL_SELECT_BLINK_INTERVAL = 0.07f;
const float MAP_PREVIEW_HEIGHT = 250.0f;
const float MAP_PREVIEW_LENGTH = 400.0f;
const Vec2<float> LVL_SELECT_POINTER_OFFSET = { MAP_PREVIEW_LENGTH / 3.0f, MAP_PREVIEW_HEIGHT / 3.0f };

///////////////////////////////////////////////////////////////////////////
// Button pointer variables
bool lvlSelectIsTransitioning = false;
bool lvlSelectIsBlinking = false;					// Use to toggle opacity of pointer to mimic blinking.

float lvlSelectCurrBlinkInterval = 0;
float lvlSelectCurrTransitionTime = 0;

///////////////////////////////////////////////////////////////////////////
// Level Preview Class
LevelPreview::LevelPreview(float x_pos, float y_pos, float x_size, float y_size, 
						   std::string const& mapFilePath, std::string const& _name) {
	// Initialize transform (pos and size) of the preview
	transform.pos.x = x_pos;
	transform.pos.y = y_pos;
	transform.size.x = x_size;
	transform.size.y = y_size;
	
	// Initialize map preview part
	GridManager::cell *mapData = FileIOManager::LoadGridFromFile(mapFilePath);

	int mapX = GridManager::gridX, mapY = GridManager::gridY;
	int minX = 0, minY = 0, maxX = 0, maxY = 0;

	// Set the bounds of the map
	for (int index = 0; index < mapX * mapY; ++index) { // Min Y
		if (mapData[index].isRenderable) {
			minY = index / mapX;

			if (minY > 0) minY -= 1;
			break;
		}
	}

	for (int index = mapX * mapY - 1; index > 0; --index) { // Max Y
		if (mapData[index].isRenderable) {
			maxY = index / mapX + 1;

			if (maxY < mapY) maxY += 1;
			break;
		}
	}

	for (int index_x = 0; index_x < mapX; ++index_x) { // Min X
		for (int index_y = 0; index_y < mapX * mapY; index_y += mapX) {
			if (mapData[index_x + index_y].isRenderable) {
				minX = index_x;

				if (minX > 0) minX -= 1;
				break;
			}
		}
		if (minX) break;
	}

	for (int index_x = mapX - 1; index_x > 0; --index_x) { // Max X
		for (int index_y = 0; index_y < mapX * mapY; index_y += mapX) {
			if (mapData[index_x + index_y].isRenderable) {
				maxX = index_x + 1;

				if (maxX < mapX) maxX += 1;
				break;
			}
		}
		if (maxX) break;
	}

	// Read the mapdata within the smaller bounds
	for (int index_y = minY; index_y < maxY; ++index_y) {
		for (int index_x = minX; index_x < maxX; ++index_x) {
			if (!mapData[index_x + index_y * mapX].isRenderable) {
				map.push_back(0);
			}
			else if (mapData[index_x + index_y * mapX]._building.data.type == BuildingEnum::NONE) {
				map.push_back(1);
			}
			else if (mapData[index_x + index_y * mapX]._building.data.type == BuildingEnum::NATURE) {
				map.push_back(2);
			}
			else {
				map.push_back(3);
			}
		}
	}

	delete[] mapData;

	mapSizeX = maxX - minX;
	int mapSizeY = maxY - minY;
	cellSize = transform.size.y / (float)mapSizeY * 0.9f;
	startingCellPos.x = transform.pos.x + transform.size.x * 0.5f - ((float)mapSizeX * 0.5f * cellSize);
	startingCellPos.y = transform.pos.y - transform.size.y * 0.5f + ((float)mapSizeY * 0.5f * cellSize);

	// Initialize preview name
	name = UI::TextBox({ x_pos, y_pos - y_size * 0.9f }, _name, UI::CENTER_JUSTIFY, x_size, 30.0f, COLOR_BLACK);
	// Debug size: std::to_string(minX) + " " + std::to_string(maxX) + " " + std::to_string(minY) + " " + std::to_string(maxY)
}

void LevelPreview::Render() {
	RenderSystem::AddRectToBatch(RenderSystem::UI_BATCH, transform.pos.x + transform.size.x * 0.025f, transform.pos.y - transform.size.y * 0.05f, transform.size.x * 0.95f, transform.size.y * 0.9f, COLOR_PREVIEW_WATER, 2);

	float x, y;
	for (int index = 0; index < map.size(); ++index) {
		if (map[index]) {
			switch (map[index]) {
			case 1:
				RenderSystem::AddRectToBatch(RenderSystem::UI_BATCH, startingCellPos.x + (index % mapSizeX) * cellSize, startingCellPos.y - (index / mapSizeX) * cellSize,
											 cellSize, cellSize, COLOR_PREVIEW_LAND, 3);
			break;
			case 2:
				RenderSystem::AddRectToBatch(RenderSystem::UI_BATCH, startingCellPos.x + (index % mapSizeX) * cellSize, startingCellPos.y - (index / mapSizeX) * cellSize,
											 cellSize, cellSize, COLOR_PREVIEW_NATURE, 3);
			break;
			}
		}
	}

	name.Render();
}

///////////////////////////////////////////////////////////////////////////
// Scene Functions

void SceneLevelSelect::Load() {

}

void SceneLevelSelect::Initialize() {
	LvlSelectInitializeUI();
	InputManager::SubscribeToKey(AEVK_LBUTTON, InputManager::TRIGGERED, HandleLvlSelectBtnClick);

	lvlSelectCurrBlinkInterval = LVL_SELECT_BLINK_INTERVAL;
	lvlSelectCurrTransitionTime = LVL_SELECT_TRANSITION_TIME;
	lvlSelectIsTransitioning = false;
	lvlSelectCurrBlinkInterval = false;

	AudioManager::PlayBGM(AudioManager::ClipName::BGM_MAIN);
}

void SceneLevelSelect::Update() {
	InputManager::HandleInput();
	AudioManager::Update();
}

void SceneLevelSelect::Draw() {
	RenderSystem::AddRectToBatch(RenderSystem::UI_BATCH, AEGfxGetWinMinX(), -AEGfxGetWinMinY(), (float)AEGetWindowWidth(), (float)AEGetWindowHeight(), TextureManager::CREDITS_BG, -1);
	
	HandleLvlSelectBtnHover();
	
	DrawLvlSelectButtons();
	for (LevelPreview preview : lvlPreviews) {
		preview.Render();
	}


	RenderSystem::Render();
}

void SceneLevelSelect::Free() {
	InputManager::Free();
	RenderSystem::Free();

	lvlSelectButtons.clear();
	lvlPreviews.clear();
}

void SceneLevelSelect::Unload() {
	
}

///////////////////////////////////////////////////////////////////////////
// UI Functions

void LvlSelectInitializeUI() {
	// BACK BUTTON
	lvlSelectBackBtn.render.rect.graphics.tex = TextureManager::BACK_BTN;
	lvlSelectBackBtn.func = LvlSelectReturnToMenu;

	lvlSelectBackBtn.render.rect.transform.size.x = 150.0f;
	lvlSelectBackBtn.render.rect.transform.size.y = 78.0f;

	lvlSelectBackBtn.render.rect.transform.pos.x = GetWorldXByPercentage(88.0f);
	lvlSelectBackBtn.render.rect.transform.pos.y = GetWorldYByPercentage(13.2f);

	lvlSelectBackBtn.render.rect.transform.cachedSize = lvlSelectBackBtn.render.rect.transform.size;
	lvlSelectButtons.push_back(lvlSelectBackBtn);

	// LAST SAVED GAME BUTTON
	lastMapBtn.render.rect.graphics.tex = TextureManager::BLANK_PROMPT;
	// lastMapBtn.func = LvlSelectReturnToMenu;

	lastMapBtn.render.rect.transform.size.x = MAP_PREVIEW_LENGTH;
	lastMapBtn.render.rect.transform.size.y = MAP_PREVIEW_HEIGHT;

	lastMapBtn.render.rect.transform.pos.x = GetWorldXByPercentage(35.0f) - lastMapBtn.render.rect.transform.size.x * 0.5f;
	lastMapBtn.render.rect.transform.pos.y = GetWorldYByPercentage(80.0f) + lastMapBtn.render.rect.transform.size.y * 0.5f;

	lastMapBtn.render.rect.transform.cachedSize = lastMapBtn.render.rect.transform.size;
	lvlSelectButtons.push_back(lastMapBtn);

	LevelPreview lastMapPreview{ lastMapBtn.render.rect.transform.pos.x, lastMapBtn.render.rect.transform.pos.y,
								 lastMapBtn.render.rect.transform.size.x, lastMapBtn.render.rect.transform.size.y,
								 "Assets/JSON_Data/Maps/lastSaved.momomaps", "Last Saved" };
	lvlPreviews.push_back(lastMapPreview);

	// MAP 0 BUTTON
	lvl1Btn.render.rect.graphics.tex = TextureManager::BLANK_PROMPT;
	lvl1Btn.func = LvlSelectLoadMap1;

	lvl1Btn.render.rect.transform.size.x = MAP_PREVIEW_LENGTH;
	lvl1Btn.render.rect.transform.size.y = MAP_PREVIEW_HEIGHT;

	lvl1Btn.render.rect.transform.pos.x = GetWorldXByPercentage(65.0f) - lvl1Btn.render.rect.transform.size.x * 0.5f;
	lvl1Btn.render.rect.transform.pos.y = GetWorldYByPercentage(80.0f) + lvl1Btn.render.rect.transform.size.y * 0.5f;

	lvl1Btn.render.rect.transform.cachedSize = lvl1Btn.render.rect.transform.size;
	lvlSelectButtons.push_back(lvl1Btn);

	LevelPreview map0Preview{ lvl1Btn.render.rect.transform.pos.x, lvl1Btn.render.rect.transform.pos.y,
							  lvl1Btn.render.rect.transform.size.x, lvl1Btn.render.rect.transform.size.y,
							  "Assets/JSON_Data/Maps/map0.momomaps", "Archipelago" };
	lvlPreviews.push_back(map0Preview);

	// MAP 1 BUTTON
	lvl2Btn.render.rect.graphics.tex = TextureManager::BLANK_PROMPT;
	lvl2Btn.func = LvlSelectLoadMap2;

	lvl2Btn.render.rect.transform.size.x = MAP_PREVIEW_LENGTH;
	lvl2Btn.render.rect.transform.size.y = MAP_PREVIEW_HEIGHT;

	lvl2Btn.render.rect.transform.pos.x = GetWorldXByPercentage(35.0f) - lvl2Btn.render.rect.transform.size.x * 0.5f;
	lvl2Btn.render.rect.transform.pos.y = GetWorldYByPercentage(50.0f) + lvl2Btn.render.rect.transform.size.y * 0.5f;

	lvl2Btn.render.rect.transform.cachedSize = lvl2Btn.render.rect.transform.size;
	lvlSelectButtons.push_back(lvl2Btn);

	LevelPreview map1Preview{ lvl2Btn.render.rect.transform.pos.x, lvl2Btn.render.rect.transform.pos.y,
						      lvl2Btn.render.rect.transform.size.x, lvl2Btn.render.rect.transform.size.y,
							  "Assets/JSON_Data/Maps/map1.momomaps", "Big Donut" };
	lvlPreviews.push_back(map1Preview);

	// MAP 2 BUTTON
	lvl3Btn.render.rect.graphics.tex = TextureManager::BLANK_PROMPT;
	lvl3Btn.func = LvlSelectLoadMap3;

	lvl3Btn.render.rect.transform.size.x = MAP_PREVIEW_LENGTH;
	lvl3Btn.render.rect.transform.size.y = MAP_PREVIEW_HEIGHT;

	lvl3Btn.render.rect.transform.pos.x = GetWorldXByPercentage(65.0f) - lvl3Btn.render.rect.transform.size.x * 0.5f;
	lvl3Btn.render.rect.transform.pos.y = GetWorldYByPercentage(50.0f) + lvl3Btn.render.rect.transform.size.y * 0.5f;

	lvl3Btn.render.rect.transform.cachedSize = lvl3Btn.render.rect.transform.size;
	lvlSelectButtons.push_back(lvl3Btn);

	LevelPreview map2Preview{ lvl3Btn.render.rect.transform.pos.x, lvl3Btn.render.rect.transform.pos.y,
							  lvl3Btn.render.rect.transform.size.x, lvl3Btn.render.rect.transform.size.y,
							  "Assets/JSON_Data/Maps/map2.momomaps", "Spiral Island" };
	lvlPreviews.push_back(map2Preview);

	// USER MAP 0 BUTTON
	lvlUser1Btn.render.rect.graphics.tex = TextureManager::BLANK_PROMPT;
	lvlUser1Btn.func = LvlSelectLoadUserMap1;

	lvlUser1Btn.render.rect.transform.size.x = MAP_PREVIEW_LENGTH;
	lvlUser1Btn.render.rect.transform.size.y = MAP_PREVIEW_HEIGHT;

	lvlUser1Btn.render.rect.transform.pos.x = GetWorldXByPercentage(35.0f) - lvlUser1Btn.render.rect.transform.size.x * 0.5f;
	lvlUser1Btn.render.rect.transform.pos.y = GetWorldYByPercentage(20.0f) + lvlUser1Btn.render.rect.transform.size.y * 0.5f;

	lvlUser1Btn.render.rect.transform.cachedSize = lvlUser1Btn.render.rect.transform.size;
	lvlSelectButtons.push_back(lvlUser1Btn);

	LevelPreview userMap0Preview{ lvlUser1Btn.render.rect.transform.pos.x, lvlUser1Btn.render.rect.transform.pos.y,
								  lvlUser1Btn.render.rect.transform.size.x, lvlUser1Btn.render.rect.transform.size.y,
								  "Assets/JSON_Data/Maps/userMap0.momomaps", "User Map 1" };
	lvlPreviews.push_back(userMap0Preview);

	// USER MAP 1 BUTTON
	lvlUser2Btn.render.rect.graphics.tex = TextureManager::BLANK_PROMPT;
	lvlUser2Btn.func = LvlSelectLoadUserMap2;

	lvlUser2Btn.render.rect.transform.size.x = MAP_PREVIEW_LENGTH;
	lvlUser2Btn.render.rect.transform.size.y = MAP_PREVIEW_HEIGHT;

	lvlUser2Btn.render.rect.transform.pos.x = GetWorldXByPercentage(65.0f) - lvlUser2Btn.render.rect.transform.size.x * 0.5f;
	lvlUser2Btn.render.rect.transform.pos.y = GetWorldYByPercentage(20.0f) + lvlUser2Btn.render.rect.transform.size.y * 0.5f;

	lvlUser2Btn.render.rect.transform.cachedSize = lvlUser2Btn.render.rect.transform.size;
	lvlSelectButtons.push_back(lvlUser2Btn);

	LevelPreview userMap1Preview{ lvlUser2Btn.render.rect.transform.pos.x, lvlUser2Btn.render.rect.transform.pos.y,
								  lvlUser2Btn.render.rect.transform.size.x, lvlUser2Btn.render.rect.transform.size.y,
								  "Assets/JSON_Data/Maps/userMap1.momomaps", "User Map 2" };
	lvlPreviews.push_back(userMap1Preview);
}

void DrawLvlSelectButtons() {
	// Loop through all buttons and draw them.
	for (RenderSystem::Interactable& btn : lvlSelectButtons) {
		// Skip if button is not visible.
		if (!btn.isActive) continue;

		RenderSystem::AddRectToBatch(RenderSystem::UI_BATCH, btn.render.rect.transform.pos.x, btn.render.rect.transform.pos.y, btn.render.rect.transform.size.x, btn.render.rect.transform.size.y, btn.render.rect.graphics.tex, btn.render.layer);
	}
}

void HandleLvlSelectBtnHover() {
	// Handle pointer blinking when transitioning to a different scene.
	if (lvlSelectIsTransitioning) {
		// Transition timer.
		if (lvlSelectCurrTransitionTime > 0) {
			// Tick timer to transition to next scene.
			lvlSelectCurrTransitionTime -= (float)AEFrameRateControllerGetFrameTime();
		}
		else {
			// Transition to next scene.
			lvlSelectClickedBtn.func();
		}

		// Blink timer.
		if (lvlSelectCurrBlinkInterval > 0) {
			// Tick timer to blink.
			lvlSelectCurrBlinkInterval -= (float)AEFrameRateControllerGetFrameTime();
		}
		else {
			// Toggle blink.
			lvlSelectIsBlinking = !lvlSelectIsBlinking;
			lvlSelectCurrBlinkInterval = LVL_SELECT_BLINK_INTERVAL;
		}

		// Change opacity of render setting to mimic blinking.
		RenderSystem::SetRenderSetting(Vec4<float>{1, 1, 1, static_cast<float>(lvlSelectIsBlinking)});
		// Draw pointer blinking.
		RenderSystem::AddRectToBatch(RenderSystem::UI_BATCH, lvlSelectClickedBtn.render.rect.transform.pos.x - LVL_SELECT_POINTER_OFFSET.x, lvlSelectClickedBtn.render.rect.transform.pos.y - LVL_SELECT_POINTER_OFFSET.y, 60, 90, TextureManager::POINTER, 2);
		return;
	}

	// Cache mouse position.
	Vec2<int> mousePos = InputManager::GetMousePos();

	// Convert to world space position.
	mousePos.x -= (int)AEGfxGetWinMaxX();
	mousePos.y -= (int)AEGfxGetWinMaxY();

	// Loop through all buttons.
	for (RenderSystem::Interactable& btn : lvlSelectButtons) {
		// Skip if button is not visible or clickable.
		if (!btn.isActive || !btn.isClickable) continue;

		// Check if mouse is hovering button.
		if (MouseInsideButton(mousePos, btn.render.rect.transform.pos, btn.render.rect.transform.size)) {
			// Draw pointer.
			RenderSystem::AddRectToBatch(RenderSystem::UI_BATCH, btn.render.rect.transform.pos.x - LVL_SELECT_POINTER_OFFSET.x, btn.render.rect.transform.pos.y - LVL_SELECT_POINTER_OFFSET.y, 60, 90, TextureManager::POINTER, 3);
		}
	}
}

void HandleLvlSelectBtnClick() {
	// Prevent any click if transitioning.
	if (lvlSelectIsTransitioning) return;

	// Cache mouse position.
	Vec2<int> mousePos = InputManager::GetMousePos();

	// Convert to world space position.
	mousePos.x -= (int)AEGfxGetWinMaxX();
	mousePos.y -= (int)AEGfxGetWinMaxY();

	// Loop through all buttons.
	for (RenderSystem::Interactable& btn : lvlSelectButtons) {
		// Skip if button is not visible or clickable.
		if (!btn.isActive || !btn.isClickable) continue;

		// Check if mouse is hovering button.
		if (MouseInsideButton(mousePos, btn.render.rect.transform.pos, btn.render.rect.transform.size)) {
			// Cache button.
			lvlSelectClickedBtn = btn;

			switch (lvlSelectClickedBtn.render.rect.graphics.tex)
			{
			case TextureManager::QUIT_BTN:
			case TextureManager::YES_BTN:
			case TextureManager::NO_BTN:
				// Immediate calling of button function when clicking. No transition needed. (For the above buttons)
				lvlSelectClickedBtn.func();
				break;
			default:
				// Play blinking pointer transition.
				lvlSelectIsTransitioning = true;
				break;
			}
			break;
		}
	}
}

///////////////////////////////////////////////////////////////////////////
// Button functions

void LvlSelectReturnToMenu() {
	SceneManager::LoadScene(SceneManager::MAIN_MENU);
}

void LvlSelectLoadMap1() {
	MapToLoad("Assets/JSON_Data/Maps/map0.momomaps");
	SceneManager::LoadScene(SceneManager::GAME_LEVEL);
}

void LvlSelectLoadMap2() {
	MapToLoad("Assets/JSON_Data/Maps/map1.momomaps");
	SceneManager::LoadScene(SceneManager::GAME_LEVEL);
}

void LvlSelectLoadMap3() {
	MapToLoad("Assets/JSON_Data/Maps/map2.momomaps");
	SceneManager::LoadScene(SceneManager::GAME_LEVEL);
}

void LvlSelectLoadUserMap1() {
	MapToLoad("Assets/JSON_Data/Maps/userMap0.momomaps");
	SceneManager::LoadScene(SceneManager::GAME_LEVEL);
}

void LvlSelectLoadUserMap2() {
	MapToLoad("Assets/JSON_Data/Maps/userMap1.momomaps");
	SceneManager::LoadScene(SceneManager::GAME_LEVEL);
}
